from __future__ import print_function
import os
import pickle
import datetime
import requests
import json

from google_auth_oauthlib.flow import InstalledAppFlow
from google.auth.transport.requests import Request
from googleapiclient.discovery import build

#: Google Fit API scope for reading activity data.
SCOPES = ['https://www.googleapis.com/auth/fitness.activity.read']
#: File to store daily step data.
STEP_DATA_FILE = 'daily_steps.json'

def load_step_data():
    """
    @brief Load existing step data from file.
    @return dict: Dictionary of date to step count.
    """
    if os.path.exists(STEP_DATA_FILE):
        with open(STEP_DATA_FILE, 'r') as f:
            return json.load(f)
    return {}

def save_step_data(data):
    """
    @brief Save step data to file.
    @param data Dictionary of date to step count.
    """
    with open(STEP_DATA_FILE, 'w') as f:
        json.dump(data, f, indent=2)

def get_current_date():
    """
    @brief Get today's date as string (YYYY-MM-DD).
    @return str: Current date.
    """
    return datetime.datetime.now().strftime("%Y-%m-%d")

def steps_to_lives(steps):
    """
    @brief Convert steps to lives.
    @param steps Number of steps.
    @return int: Number of lives.
    """
    return steps // 5

def get_google_fit_service():
    """
    @brief Authenticate with Google Fit API and return a service object.
    @return googleapiclient.discovery.Resource: Google Fit service object.
    """
    creds = None
    token_path = 'token.pickle'
    credentials_path = './[credentials_file_name].json'
    if os.path.exists(token_path):
        with open(token_path, 'rb') as token:
            creds = pickle.load(token)
    if not creds or not creds.valid:
        if creds and creds.expired and creds.refresh_token:
            creds.refresh(Request())
        else:
            flow = InstalledAppFlow.from_client_secrets_file(credentials_path, SCOPES)
            creds = flow.run_local_server(port=8080)
        with open(token_path, 'wb') as token:
            pickle.dump(creds, token)
    return build('fitness', 'v1', credentials=creds)

def get_steps_from_google_fit():
    """
    @brief Retrieve the number of steps for the current day from Google Fit.
    @return int: Total steps for today.
    """
    service = get_google_fit_service()
    now = datetime.datetime.now(datetime.timezone.utc)
    start = datetime.datetime(now.year, now.month, now.day, tzinfo=datetime.timezone.utc)
    start_time_millis = int(start.timestamp() * 1000)
    end_time_millis = int(now.timestamp() * 1000)
    body = {
        "aggregateBy": [{
            "dataTypeName": "com.google.step_count.delta"
        }],
        "bucketByTime": {"durationMillis": 86400000},
        "startTimeMillis": start_time_millis,
        "endTimeMillis": end_time_millis
    }
    response = service.users().dataset().aggregate(userId="me", body=body).execute()
    steps = sum(
        value.get('intVal', 0)
        for bucket in response.get('bucket', [])
        for dataset in bucket.get('dataset', [])
        for point in dataset.get('point', [])
        for value in point.get('value', [])
    )
    print(f"Retrieved {steps} steps from Google Fit.")
    return steps

def send_to_particle_photon(lives):
    """
    @brief Send the number of lives to the Particle Photon 2 using Particle Cloud API.
    @param lives Number of lives to send.
    """
    device_id = DEVICE_ID
    function_name = "addLives"
    access_token = ACCESS_TOKEN
    url = f"https://api.particle.io/v1/devices/{device_id}/{function_name}"
    payload = {
        "arg": str(lives),
        "access_token": access_token
    }
    try:
        response = requests.post(url, data=payload, timeout=10)
        response.raise_for_status()
        print("Successfully sent lives to Particle Photon:", response.json())
    except requests.RequestException as e:
        print("Failed to send lives:", e)

def main():
    """
    @brief Main function to synchronize Google Fit steps with Particle Photon lives.
    """
    step_data = load_step_data()
    today = get_current_date()
    current_steps = get_steps_from_google_fit()
    previous_steps = step_data.get(today, 0)
    step_difference = current_steps - previous_steps if previous_steps else current_steps

    if previous_steps:
        print(f"Previous steps today: {previous_steps}, New steps: {current_steps}")
    else:
        print(f"First reading today: {current_steps} steps")

    if step_difference > 0:
        lives = steps_to_lives(current_steps)
        send_to_particle_photon(lives)
        step_data[today] = current_steps
        save_step_data(step_data)
    else:
        print("No new steps since last check")

if __name__ == "__main__":
    main()
