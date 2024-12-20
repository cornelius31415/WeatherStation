"""
To classify weather data: https://www.timeanddate.de/wetter/deutschland/muenchen/rueckblick?month=11&year=2024

"""




import pandas as pd
import matplotlib.pyplot as plt

# Datei einlesen
file_path = 'WEATHER_(29102024-02112024).CSV'
df = pd.read_csv(file_path)
df = df.drop(columns = ["Altitude BMP1 (m), Altitude BMP2 (m)"])

date_column = 'Time'




df.set_index(date_column, inplace=True)


df.plot(subplots=True, figsize=(10, 8), title='Weather Data')
plt.xlabel('Date')
plt.tight_layout()
plt.show()
