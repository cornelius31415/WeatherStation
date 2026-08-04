#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Apr 25 00:47:45 2025

@author: cornelius
"""

# from NeuralNetworkClass import NeuralNetwork
from sklearn.model_selection import train_test_split
import pandas as pd
from sklearn.preprocessing import LabelEncoder
from sklearn.metrics import accuracy_score
from sklearn.preprocessing import StandardScaler
import numpy as np
from DeepNNClass import NeuralNetwork

# Daten laden
df = pd.read_csv("WeatherDataWithCategoriesDec24.csv")
print(df)

# Label Encoding
label_encoder = LabelEncoder()
df["Category"] = label_encoder.fit_transform(df["Category"])

# Features und Labels trennen
features = df.drop(columns=["Time", "Category"])
labels = df["Category"]

# Label-Zuordnungen anzeigen
label_mapping = dict(zip(label_encoder.classes_, label_encoder.transform(label_encoder.classes_)))
print("Label-Zuweisungen:", label_mapping)


# Train/Test-Split
X_train, X_test, y_train, y_test = train_test_split(
    features, labels, test_size=0.2, random_state=42
)

# Feature Scaling - WICHTIG: Nur auf Training fitten
scaler = StandardScaler()
X_train = scaler.fit_transform(X_train)
X_test = scaler.transform(X_test)

# Neural Network erstellen
n_classes = len(np.unique(y_train))



# nn = NeuralNetwork(input_nodes=3, hidden_nodes=200, output_nodes=n_classes, learning_rate=0.03)

# # Netz trainieren
# nn.fit(X_train, y_train, epochs=100)

nn = NeuralNetwork(3, 0.05)
nn.layer(100)
nn.layer(100)
nn.layer(n_classes)
nn.fit(X_train,y_train,epochs=100)



# Vorhersage
prediction = nn.predict(X_test)

# Accuracy ausgeben
print("Neural Network Accuracy:", accuracy_score(y_test, prediction))

# Zahlenlabels zurück in Kategorienamen konvertieren
actual_labels = label_encoder.inverse_transform(y_test.values)
predicted_labels = label_encoder.inverse_transform(prediction)

# DataFrame mit tatsächlichen und vorhergesagten Kategorienamen erstellen
results_df = pd.DataFrame({
    "Actual": actual_labels,
    "Prediction": predicted_labels
})

# Einstellungen für die Anzeige anpassen
pd.set_option('display.max_rows', 200)       # Zeige bis zu 100 Zeilen
pd.set_option('display.max_columns', None)   # Alle Spalten
pd.set_option('display.width', None)         # Keine automatische Zeilenumbrüche
pd.set_option('display.max_colwidth', None)  # Volle Länge der Texte

# DataFrame ausgeben
print(results_df.head(200))

