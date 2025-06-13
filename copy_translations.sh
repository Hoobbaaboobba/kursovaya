#!/bin/bash

# Create translations directory in build if it doesn't exist
mkdir -p build/translations
 
# Copy translation files
cp translations/buyerdatabase_*.ts build/translations/ 