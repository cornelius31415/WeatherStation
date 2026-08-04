#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Apr 25 00:43:01 2025

@author: cornelius
"""

import pandas as pd

# Dateien laden
classes_file = 'Weather4ClassesDec24.csv'
weather_file = 'WeatherStationDec24.csv'

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
output_file = 'WeatherDataWithCategoriesDec24.csv'
weather_data.to_csv(output_file, index=False)
print(f"Datei erfolgreich erstellt: {output_file}")
