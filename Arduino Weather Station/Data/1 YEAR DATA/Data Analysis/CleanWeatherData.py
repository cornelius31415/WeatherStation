#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Apr 22 14:52:16 2026

@author: cornelius
"""

import pandas as pd

# CSV einlesen
df = pd.read_csv("LabelledWeatherData_2025_only.csv")

# Zeilen filtern:
# - Category darf nicht NaN sein
# - Category darf nicht "unknown" sein
df_clean = df[df["Category"].notna() & (df["Category"] != "unknown")]

# Neue CSV speichern
df_clean.to_csv("WeatherData_cleaned.csv", index=False)

print("Gefilterte Datei gespeichert als: WeatherData_cleaned.csv")