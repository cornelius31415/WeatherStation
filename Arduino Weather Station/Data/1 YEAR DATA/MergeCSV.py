#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Mar 12 14:10:24 2025

@author: cornelius
"""

import os
import pandas as pd

def merge_csv_files(folder_path, date_column, output_file):
    all_files = [f for f in os.listdir(folder_path) if f.endswith(".csv")]
    print(all_files)
    dataframes = []
    for file in all_files:
        file_path = os.path.join(folder_path, file)
        df = pd.read_csv(file_path)
        dataframes.append(df)
    
    merged_df = pd.concat(dataframes, ignore_index=True)
    print(merged_df.columns)
    
    merged_df[date_column] = merged_df[date_column].astype(str)
    
    merged_df[date_column] = pd.to_datetime(merged_df[date_column], errors="coerce")
    merged_df = merged_df.sort_values(by=date_column)
    
    merged_df.to_csv(output_file, index=False)
    print(f"Merged CSV saved as {output_file}")

# Beispielaufruf
folder_path = "WeatherLabels"  # Pfad zum Ordner mit CSV-Dateien
output_file = "WeatherCategories24to25.csv"    # Name der Ausgabedatei
date_column = 'Time'  # Name der Spalte mit den Datumswerten
merge_csv_files(folder_path, date_column, output_file)


