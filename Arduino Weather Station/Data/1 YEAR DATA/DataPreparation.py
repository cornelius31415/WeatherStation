#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Apr 25 15:56:16 2025

@author: cornelius
"""

"""

                1. FIXING DATA FROM THE INTERNET
    
    on https://www.timeanddate.de/wetter/deutschland/muenchen/rueckblick?month=1&year=2025
    I manually copied the weather classifications of each half hour of the day into the file
    WeatherClassesDec24.numbers 
    
    Tasks:
    
    -> turn to csv
    -> correct the time format
    
    





"""

import pandas as pd
import re
from datetime import datetime

# === Einstellungen ===
input_datei = "WeatherCategories25.csv"
output_datei = "WeatherCategories25_fixed.csv"

# === Datei einlesen ===
df_raw = pd.read_csv(input_datei, header=None, sep=";", engine='python')
df_raw = df_raw.dropna(how='all', axis=1)  # Leere Spalten entfernen

# === Datum extrahieren und ergänzen ===
aktuelles_datum = None
daten = []

for _, row in df_raw.iterrows():
    erste_zelle = row[0]
    
    if not isinstance(erste_zelle, str):
        daten.append([None] + row.tolist())
        continue

    parts = erste_zelle.split("\n")

    if len(parts) == 2:
        zeit_str, datum_str = parts
        # Datum wie "So, 1. Dez" extrahieren
        match = re.search(r'(\d{1,2})\. (\w+)', datum_str)
        if match:
            tag = int(match.group(1))
            monat_name = match.group(2).lower()
            monate = {
                'jan': 1, 'feb': 2, 'mär': 3, 'apr': 4,
                'mai': 5, 'jun': 6, 'jul': 7, 'aug': 8,
                'sep': 9, 'okt': 10, 'nov': 11, 'dez': 12
            }
            monat = monate.get(monat_name[:3], 12)
            aktuelles_datum = datetime(2025, monat, tag)
    else:
        zeit_str = parts[0]

    if aktuelles_datum:
        try:
            zeit = datetime.strptime(zeit_str.strip(), "%H:%M").time()
            voll_datum = datetime.combine(aktuelles_datum, zeit)
        except ValueError:
            voll_datum = None
    else:
        voll_datum = None

    daten.append([voll_datum] + row.tolist())

# === In DataFrame umwandeln und speichern ===
spalten = ["Datetime"] + [f"Col{i}" for i in range(1, len(df_raw.columns) + 1)]
df_neu = pd.DataFrame(daten, columns=spalten)
df_neu.to_csv(output_datei, index=False)

print(f"✅ Neue Datei gespeichert: {output_datei}")



"""

           MERGING WITH DATA FROM WEATHER STATION
        



"""

# Dateien laden
classes_file = 'WeatherCategories25_fixed.csv'
weather_file = 'WeatherDataDez24toJan26.csv'

# Wetterklassen laden
weather_classes = pd.read_csv(classes_file)
weather_classes['Datetime'] = pd.to_datetime(weather_classes['Datetime'])

# Optional: Wenn die Datei 'Col3' enthält, aber noch keine 'categories' Spalte
if 'categories' not in weather_classes.columns and 'Col3' in weather_classes.columns:
    categories = weather_classes['Col3'].tolist()
    new_categories = []

    sunny = ['Heiter.', 'Teilweise sonnig.', 'Sonnig.']
    rainy = ['Leichter Regen. Stark bewölkt.', 'Leichter Regen. Niedrige Wolken.', 'Nieselregen. Bedeckt.',
              'Regen. Meist bewölkt.', 'Nieselregen. Wolkig.', 'Regenschauer. Meist bewölkt.',
              'Einzelne Schauer. Vorüberziehende Wolken.', 'Nieselschauer. Stark bewölkt.',
              'Einzelne Schauer. Wolkig.', 'Nieselschauer. Wolkig.', 'Einzelne Schauer. Teilweise sonnig.',
              'Nieselregen. Stark bewölkt.', 'Nieselschauer. Meist bewölkt.', 'Leichter Regen. Bewölkt.',
              'Nieselregen. Nebel.', 'Gewitterschauer. Heiter.', 'Nieselregen. Bewölkt.',
              'Nieselschauer. Niedrige Wolken.', 'Leichter gefrierender Regen. Bedeckt.',
              'Leichter Regen. Vorüberziehende Wolken.', 'Einzelne Schauer. Meist bewölkt.',
              'Nieselregen. Meist bewölkt.', 'Regenschauer. Bewölkt.', 'Einzelne Schauer. Stark bewölkt.',
              'Nieselschauer. Bedeckt.', 'Nieselregen. Niedrige Wolken.', 'Nebel.',
              'Leichter Regen. Bedeckt.', 'Dichter Nebel.', 'Leichter gefrierender Regen. Meist bewölkt.',
              'Leichter Nebel.', 'Leichter Regen. Meist bewölkt.', 'Einzelne Schauer. Bedeckt.',
              'Einzelne Schauer. Teilweise bewölkt.']

    snowy = ['Leichter Schneefall. Stark bewölkt.', 'Leichte Schneeschauer. Meist bewölkt.',
              'Schneeschauer. Eisnebel.', 'Leichter Schneefall. Nebel.', 'Leichter Schneefall. Meist bewölkt.',
              'Leichter Schneefall. Niedrige Wolken.', 'Leichter Schneefall. Bedeckt.',
              'Leichter Schneefall. Bewölkt.', 'Leichter Schneefall. Eisnebel.',
              'Leichter Schneefall. Vorüberziehende Wolken.', 'Schneeschauer. Nebel.',
              'Leichte Schneeschauer. Bedeckt.', 'Leichte Schneeschauer. Eisnebel.',
              'Schneeschauer. Bedeckt.', 'Leichter Schneefall. Teilweise bewölkt.',
              'Leichte Schneeschauer. Bewölkt.', 'Leichte Schneeschauer. Teilweise sonnig.',
              'Leichte Schneeschauer. Stark bewölkt.']

    cloudy = ['Teilweise bewölkt.', 'Stark bewölkt.', 'Niedrige Wolken.', 'Vorüberziehende Wolken.',
              'Wolkig.', 'Bedeckt.', 'Eisnebel.', 'Bewölkt.', 'Meist bewölkt.', 'Gewitterschauer. Bewölkt.']

    for c in categories:
        if c in sunny:
            new_categories.append('sunny')
        elif c in rainy:
            new_categories.append('rainy')
        elif c in snowy:
            new_categories.append('snowy')
        elif c in cloudy:
            new_categories.append('cloudy')
        else:
            new_categories.append('unknown')

    weather_classes['categories'] = new_categories

# Zeitfenster anlegen für jede Kategorie (±30 Minuten)
weather_classes['Start'] = weather_classes['Datetime'] - pd.Timedelta(minutes=30)
weather_classes['End'] = weather_classes['Datetime'] + pd.Timedelta(minutes=30)

# Wetterstation laden
weather_data = pd.read_csv(weather_file)
weather_data['Time'] = pd.to_datetime(weather_data['Time'])

# Kategorie zuweisen
def find_category(time, class_df):
    match = class_df[(class_df['Start'] <= time) & (class_df['End'] > time)]
    if not match.empty:
        return match.iloc[0]['categories']
    else:
        return None

weather_data['Category'] = weather_data['Time'].apply(lambda x: find_category(x, weather_classes))
new_df = weather_data[["Time","Humidity DHT22 (%)","Temperature BMP1"," Pressure BMP1 (hPa)","Category"]]
weather_data = new_df
# Ergebnis speichern
output_file = 'WeatherDataWithCategories25.csv'
weather_data.to_csv(output_file, index=False)
print(f"Datei erfolgreich erstellt: {output_file}")

































