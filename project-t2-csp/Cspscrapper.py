import requests
import csv
import time
import re

# Function to read websites from a CSV file
def read_websites_from_csv(file_path):
    websites = []
    with open(file_path, mode='r', newline='') as file:
        reader = csv.reader(file)
        next(reader)  # Skip header row if your CSV has one
        for row in reader:
            websites.append('https://' + row[0])  # Concatenate 'https://' with each URL
    return websites

# Function to fetch CSP from HTTP header with retry logic
def fetch_csp(url, retries=3, backoff_factor=0.3):
    for i in range(retries):
        try:
            response = requests.get(url, timeout=15)
            csp_policy = response.headers.get('Content-Security-Policy')
            return csp_policy
        except (requests.exceptions.ConnectionError, requests.exceptions.Timeout) as e:
            time.sleep(backoff_factor * (2 ** i))
            print(f"Retry {i+1}/{retries} for {url}")
        except requests.exceptions.RequestException as e:
            print(f"Error fetching CSP for {url}: {e}")
            return None
    return None

# Function to parse and organize CSP into a dictionary
def parse_csp(csp):
    if not csp:
        return {}
    directives = {}
    for directive in csp.split(';'):
        parts = directive.strip().split(' ')
        if len(parts) > 1:
            directives[parts[0]] = ' '.join(parts[1:])
    return directives

# Function to extract hash and nonce from CSP header
def extract_csp_info(csp):
    if not csp:
        return [], []
    hash_values = re.findall(r"'sha256-[A-Za-z0-9+/=]+'", csp)
    nonce_values = re.findall(r"'nonce-[A-Za-z0-9+/=]+'", csp)
    return hash_values, nonce_values

csv_file_path = 'top sites - ranked_domains.csv'
websites = read_websites_from_csv(csv_file_path)

# Extended field names for CSV
fieldnames = ['URL', 'default-src', 'script-src', 'style-src', 'img-src', 'frame-src', 'media-src', 'font-src', 'child-src', 'connect-src', 'object-src', 'form-action', 'frame-ancestors', 'plugin-types', 'base-uri', 'sandbox', 'report-uri', 'report-to',  'manifest-src', 'worker-src', 'prefetch-src', 'hash_values', 'nonce_values']

# Write to CSV
with open('csp_policies_clean.csv', mode='w', newline='') as file:
    writer = csv.DictWriter(file, fieldnames=fieldnames)
    writer.writeheader()

    for website in websites:
        csp = fetch_csp(website)
        parsed_csp = parse_csp(csp)
        hash_values, nonce_values = extract_csp_info(csp)
        if parsed_csp or hash_values or nonce_values:
            # Filter the parsed_csp to only include keys that are in fieldnames
            filtered_csp = {k: parsed_csp[k] for k in parsed_csp if k in fieldnames}
            row = {'URL': website, 'hash_values': hash_values, 'nonce_values': nonce_values}
            row.update(filtered_csp)
            writer.writerow(row)

print("CSP collection complete.")
