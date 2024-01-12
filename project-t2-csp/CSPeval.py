import pandas as pd

def is_too_strict(directive_value):
    if pd.isnull(directive_value) or directive_value == 'none':
        return True
    return False

def is_too_lenient(directive_value):
    if pd.isnull(directive_value):
        return False  # Assuming NaN values are not considered too lenient
    if 'unsafe-inline' in directive_value or 'unsafe-eval' in directive_value:
        return True
    return False

def is_improperly_configured(directive_value):
    if pd.isnull(directive_value):
        return False  # Assuming NaN values are not considered improperly configured
    if 'none' in directive_value and len(directive_value.split(' ')) > 1:
        return True
    return False

def count_non_empty_directives(row):
    return sum(pd.notnull(row[col]) and row[col] != 'none' for col in directive_columns)

def list_directive_types(row):
    return ', '.join(col for col in directive_columns if pd.notnull(row[col]) and row[col] != 'none')

# New analysis functions for hash and nonce values
def has_hash_values(values):
    return len(values) > 0

def has_nonce_values(values):
    return len(values) > 0


file_path = 'csp_policies_clean.csv'
csp_policies_df = pd.read_csv(file_path)

# Update the CSP directive columns to include hash and nonce values
directive_columns = ['default-src', 'script-src', 'style-src', 'img-src', 'frame-src', 'media-src', 'font-src', 'child-src', 'connect-src', 'object-src', 'form-action', 'frame-ancestors', 'plugin-types', 'base-uri', 'sandbox', 'report-uri', 'report-to', 'manifest-src', 'worker-src', 'prefetch-src', 'hash_values', 'nonce_values']

# Apply the evaluation and analysis functions
for column in directive_columns:
    if column in ['hash_values', 'nonce_values']:
        # Handle the hash and nonce columns separately
        csp_policies_df[f'{column} Present'] = csp_policies_df[column].apply(lambda x: has_hash_values(x) if column == 'hash_values' else has_nonce_values(x))
    else:
        # Apply existing analysis functions to other columns
        csp_policies_df[f'{column} Too Strict'] = csp_policies_df[column].apply(is_too_strict)
        csp_policies_df[f'{column} Too Lenient'] = csp_policies_df[column].apply(is_too_lenient)
        csp_policies_df[f'{column} Improperly Configured'] = csp_policies_df[column].apply(is_improperly_configured)

csp_policies_df['Number of Directives'] = csp_policies_df.apply(count_non_empty_directives, axis=1)
csp_policies_df['Directive Types'] = csp_policies_df.apply(list_directive_types, axis=1)


csp_policies_df.to_csv('csp_full_analysis_results.csv', index=False)


print(csp_policies_df.head())
