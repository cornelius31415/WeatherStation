#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Apr 22 14:37:44 2026

@author: cornelius
"""

import pandas as pd

# CSV einlesen
df = pd.read_csv("WeatherDataWithCategories25.csv")

# Sicherstellen, dass die Datumsspalte als Datum erkannt wird
df["Time"] = pd.to_datetime(df["Time"])

# Nur Daten aus 2025 filtern
df_2025 = df[df["Time"].dt.year == 2025]

# Neue CSV speichern
df_2025.to_csv("WeatherData_2025_only.csv", index=False)

print("Neue Datei wurde erstellt: WeatherData_2025_only.csv")