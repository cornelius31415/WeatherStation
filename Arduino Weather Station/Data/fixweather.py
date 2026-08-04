import pandas as pd
import re
from datetime import datetime

# === Einstellungen ===
input_datei = "WeatherClassesDec24.csv"
output_datei = "WeatherClassesDec24_fixed.csv"

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
            aktuelles_datum = datetime(2024, monat, tag)
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
