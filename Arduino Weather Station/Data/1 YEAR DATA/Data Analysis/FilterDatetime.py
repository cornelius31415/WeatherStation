#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Apr 22 15:07:32 2026

@author: cornelius
"""

import pandas as pd

# Datei einlesen
df = pd.read_csv("WeatherData_cleaned.csv")

# Datumsspalte in datetime umwandeln
df["Time"] = pd.to_datetime(df["Time"])

# Neue Zeitfeatures erstellen (direkt als float)
df["year"] = df["Time"].dt.year.astype(float)
df["month"] = df["Time"].dt.month.astype(float)
df["day"] = df["Time"].dt.day.astype(float)
df["hour"] = df["Time"].dt.hour.astype(float)
df["minute"] = df["Time"].dt.minute.astype(float)

# Originale Datumsspalte entfernen
df = df.drop(columns=["Time"])

# Neue CSV speichern
df.to_csv("WeatherData_with_time_features.csv", index=False)

print("Neue Datei erstellt: WeatherData_with_time_features.csv")