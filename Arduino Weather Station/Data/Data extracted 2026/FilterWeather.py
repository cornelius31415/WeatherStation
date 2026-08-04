#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Apr 24 22:57:14 2025

@author: cornelius
"""

import pandas as pd

# === Einstellungen ===
input_datei = "WEATHER.CSV"  # Name deiner Eingabedatei
output_datei = "CleanTimestampWeather.csv"  # Name der Ausgabedatei
datumsspalte = "Time"  # Name der Spalte mit dem Datum/Zeitstempel

# === CSV einlesen ===
df = pd.read_csv(input_datei)

# === Nur gültige Zeitstempel behalten ===
df[datumsspalte] = pd.to_datetime(df[datumsspalte], errors='coerce')
df = df[df[datumsspalte].notna()]




# # === Nach Monat filtern: Dezember (12), Januar (1), Februar (2), März (3) ===
# df_gefiltert = df[(df[datumsspalte].dt.month >= 12) ]

# === Gefilterte Daten speichern ===
df.to_csv(output_datei, index=False)

# print(f"Gefilterte Datei wurde gespeichert als: {output_datei}")
