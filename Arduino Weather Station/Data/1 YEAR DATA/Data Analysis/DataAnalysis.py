"""
To classify weather data: https://www.timeanddate.de/wetter/deutschland/muenchen/rueckblick?month=11&year=2024

"""




import pandas as pd
import matplotlib.pyplot as plt

# Datei einlesen
file_path = 'WeatherClassesDec24.csv'
df = pd.read_csv(file_path)
categories = df["Col3"].values.tolist()
print(len(categories))


outputFile = "Weather4ClassesDec24.csv"


new_categories = []

sunny = ['Heiter.',
'Teilweise sonnig.',
'Sonnig.']


rainy = ['Leichter Regen. Stark bewölkt.',
'Leichter Regen. Niedrige Wolken.',
'Nieselregen. Bedeckt.',
'Regen. Meist bewölkt.',
'Nieselregen. Wolkig.',
'Regenschauer. Meist bewölkt.',
 'Einzelne Schauer. Vorüberziehende Wolken.',
 'Nieselschauer. Stark bewölkt.',
 'Einzelne Schauer. Wolkig.',
 'Nieselschauer. Wolkig.',
 'Einzelne Schauer. Teilweise sonnig.',
 'Nieselregen. Stark bewölkt.',
 'Nieselschauer. Meist bewölkt.',
 'Leichter Regen. Bewölkt.',
 'Nieselregen. Nebel.',
'Gewitterschauer. Heiter.',
'Nieselregen. Bewölkt.',
'Nieselschauer. Niedrige Wolken.',
'Leichter gefrierender Regen. Bedeckt.',
'Leichter Regen. Vorüberziehende Wolken.',
'Einzelne Schauer. Meist bewölkt.',
'Nieselregen. Meist bewölkt.',
'Regenschauer. Bewölkt.',
'Einzelne Schauer. Stark bewölkt.',
'Nieselschauer. Bedeckt.',
'Nieselregen. Niedrige Wolken.',
'Nebel.',
'Leichter Regen. Bedeckt.',
'Dichter Nebel.',
'Leichter gefrierender Regen. Meist bewölkt.',
'Leichter Nebel.',
'Leichter Regen. Meist bewölkt.',
'Einzelne Schauer. Bedeckt.',
'Einzelne Schauer. Teilweise bewölkt.']



snowy = ['Leichter Schneefall. Stark bewölkt.',
'Leichte Schneeschauer. Meist bewölkt.',
'Schneeschauer. Eisnebel.',
'Leichter Schneefall. Nebel.',
'Leichter Schneefall. Meist bewölkt.',
'Leichter Schneefall. Niedrige Wolken.',
'Leichter Schneefall. Bedeckt.',
'Leichter Schneefall. Bewölkt.',
'Leichter Schneefall. Eisnebel.',
'Leichter Schneefall. Vorüberziehende Wolken.',
'Schneeschauer. Nebel.',
'Leichte Schneeschauer. Bedeckt.',
'Leichte Schneeschauer. Eisnebel.',
'Schneeschauer. Bedeckt.',
'Leichter Schneefall. Teilweise bewölkt.',
'Leichte Schneeschauer. Bewölkt.',
'Leichte Schneeschauer. Teilweise sonnig.',
'Leichte Schneeschauer. Stark bewölkt.']

cloudy = ['Teilweise bewölkt.',
'Stark bewölkt.',
'Niedrige Wolken.',
'Vorüberziehende Wolken.',
'Wolkig.',
'Bedeckt.',
'Eisnebel.',
'Bewölkt.',
'Meist bewölkt.',
'Gewitterschauer. Bewölkt.']

for category in categories:
    if category in sunny:
        new_categories.append('sunny')
        
    elif category in rainy:
        new_categories.append('rainy')
        
    elif category in snowy:
        new_categories.append('snowy')
        
    elif category in cloudy:
        new_categories.append('cloudy')
    
    else:
        new_categories.append(category)

categories = new_categories


df['categories'] = categories
print(df["Col3"],df["categories"])

print(df)

dataframe = df[['Datetime','categories']]
print(dataframe)

dataframe.to_csv(outputFile,index=False)






















































"""


Sunny

'Heiter.'
'Teilweise sonnig.'
'Sonnig.'

Rainy

'Leichter Regen. Stark bewölkt.'
'Leichter Regen. Niedrige Wolken.'
'Nieselregen. Bedeckt.'
'Regen. Meist bewölkt.'
'Nieselregen. Wolkig.'
'Regenschauer. Meist bewölkt.'
 'Einzelne Schauer. Vorüberziehende Wolken.'
 'Nieselschauer. Stark bewölkt.'
 'Einzelne Schauer. Wolkig.'
 'Nieselschauer. Wolkig.'
 'Einzelne Schauer. Teilweise sonnig.'
 'Nieselregen. Stark bewölkt.'
 'Nieselschauer. Meist bewölkt.'
 'Leichter Regen. Bewölkt.'
 'Nieselregen. Nebel.'
Gewitterschauer. Heiter.'
'Nieselregen. Bewölkt.'
'Nieselschauer. Niedrige Wolken.'
'Leichter gefrierender Regen. Bedeckt.'
'Leichter Regen. Vorüberziehende Wolken.'
'Einzelne Schauer. Meist bewölkt.'
'Nieselregen. Meist bewölkt.'
'Regenschauer. Bewölkt.'
'Einzelne Schauer. Stark bewölkt.'
'Nieselschauer. Bedeckt.'
'Nieselregen. Niedrige Wolken.'
'Nebel.'
'Leichter Regen. Bedeckt.'
'Dichter Nebel.'
'Leichter gefrierender Regen. Meist bewölkt.'
'Leichter Nebel.'
'Leichter Regen. Meist bewölkt.'
'Einzelne Schauer. Bedeckt.'
'Einzelne Schauer. Teilweise bewölkt.'


Snowy

'Leichter Schneefall. Stark bewölkt.'
'Leichte Schneeschauer. Meist bewölkt.'
'Schneeschauer. Eisnebel.'
'Leichter Schneefall. Nebel.'
'Leichter Schneefall. Meist bewölkt.'
'Leichter Schneefall. Niedrige Wolken.'
'Leichter Schneefall. Bedeckt.'
'Leichter Schneefall. Bewölkt.'
'Leichter Schneefall. Eisnebel.'
'Leichter Schneefall. Vorüberziehende Wolken.'
'Schneeschauer. Nebel.'
'Leichte Schneeschauer. Bedeckt.'
'Leichte Schneeschauer. Eisnebel.'
'Schneeschauer. Bedeckt.'
'Leichter Schneefall. Teilweise bewölkt.'
'Leichte Schneeschauer. Bewölkt.'
'Leichte Schneeschauer. Teilweise sonnig.'
'Leichte Schneeschauer. Stark bewölkt.'


Cloudy

'Teilweise bewölkt.'
'Stark bewölkt.'
'Niedrige Wolken.'
'Vorüberziehende Wolken.'
'Wolkig.'
'Bedeckt.'
'Eisnebel.'
'Bewölkt.'
'Meist bewölkt.'
'Gewitterschauer. Bewölkt.'




"""