import pandas as pd
import matplotlib.pyplot as plt

# Load the data
csp_policies_df = pd.read_csv('csp_full_analysis_results.csv')

# Define the CSP directive columns
directive_columns = ['default-src', 'script-src', 'style-src', 'img-src', 'frame-src', 'media-src', 'font-src', 'child-src', 'connect-src', 'object-src', 'form-action', 'frame-ancestors', 'plugin-types', 'base-uri', 'sandbox', 'report-uri', 'report-to', 'manifest-src', 'worker-src', 'prefetch-src']
# Function to check for common misconfigurations
def check_common_misconfigurations(row):
    issues = []
    for col in directive_columns:
        value = row[col]
        if pd.isnull(value) or value == 'none':
            issues.append(f'{col} is too strict or missing')
        if 'unsafe-inline' in str(value) or 'unsafe-eval' in str(value):
            issues.append(f'{col} is too lenient')
        if '*' in str(value):
            issues.append(f'{col} uses wildcard, which is overly broad')
    return '; '.join(issues)


# 1. Distribution of Directives
directive_presence = csp_policies_df[directive_columns].notnull().sum()
directive_presence.sort_values(inplace=True)
directive_presence.plot(kind='barh', figsize=(10, 8))
plt.title('Distribution of CSP Directives')
plt.xlabel('Number of Websites')
plt.ylabel('Directives')
plt.show()

# 2. Trends in Leniency and Strictness
lenient_strict_columns = [col for col in csp_policies_df if 'Too Lenient' in col or 'Too Strict' in col or 'Improperly Configured' in col]
lenient_strict_data = csp_policies_df[lenient_strict_columns].sum()
lenient_strict_data.plot(kind='bar', figsize=(12, 6))
plt.title('Trends in Leniency and Strictness of CSP Directives')
plt.ylabel('Number of Websites')
plt.xticks(rotation=45, ha='right')
plt.show()

# 3. Presence of Hash and Nonce Values
hash_nonce_presence = csp_policies_df[['hash_values Present', 'nonce_values Present']].sum()
hash_nonce_presence.plot(kind='bar', figsize=(8, 6))
plt.title('Presence of Hash and Nonce Values in CSP Policies')
plt.ylabel('Number of Websites')
plt.xticks(rotation=0)
plt.show()

# Apply the function to each row
csp_policies_df['Common Misconfigurations'] = csp_policies_df.apply(check_common_misconfigurations, axis=1)

# Counting common misconfigurations
misconfig_counts = csp_policies_df['Common Misconfigurations'].value_counts()
misconfig_counts.plot(kind='bar', figsize=(12, 6))
plt.title('Common CSP Misconfigurations')
plt.ylabel('Number of Websites')
plt.xticks(rotation=45, ha='right')
plt.show()

# Reporting on potential misconfigurations or bad uses
with open('csp_misconfiguration_report.txt', 'w') as report_file:
    for index, row in csp_policies_df.iterrows():
        if row['Common Misconfigurations']:
            report_file.write(f"Website: {row['URL']}\nMisconfigurations: {row['Common Misconfigurations']}\n\n")

print("Misconfiguration report generated: csp_misconfiguration_report.txt")