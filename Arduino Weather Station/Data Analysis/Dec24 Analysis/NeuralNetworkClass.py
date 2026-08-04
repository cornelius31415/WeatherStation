#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Apr 25 00:46:22 2025

@author: cornelius
"""

import numpy as np
import scipy.special

class NeuralNetwork():
    
    def __init__(self,input_nodes, hidden_nodes, output_nodes,learning_rate):
        # initialise the basic attributes of the NeuralNetwork objekt
        self.input_nodes = input_nodes
        self.hidden_nodes = hidden_nodes
        self.output_nodes = output_nodes
        self.learning_rate = learning_rate
        
    
        self.w12 = np.random.rand(self.input_nodes,self.hidden_nodes)-0.5
        self.w23 = np.random.rand(self.hidden_nodes,self.output_nodes)-0.5
        

    def feedforward(self,input_list):
        
        input_list = np.array(input_list,ndmin=2)
        input_hidden =   np.dot(input_list, self.w12)
        output_hidden = scipy.special.expit(input_hidden)
        
        input_final = np.dot(output_hidden,self.w23)
        output_final = scipy.special.expit(input_final)
        
        return output_final
    
    def train(self,input_list, target_list):
        
        input_list = np.array(input_list,ndmin=2)
        input_hidden =   np.dot(input_list, self.w12)
        output_hidden = scipy.special.expit(input_hidden)
        
        input_final = np.dot(output_hidden,self.w23)
        output_final = scipy.special.expit(input_final)
        
        output_errors = target_list - output_final
        hidden_errors = np.dot(output_errors,np.transpose( self.w23))
        
  
        self.w23 += self.learning_rate * ( output_hidden.T @ output_errors * output_final * (1-output_final ))
        self.w12 += self.learning_rate * ( input_list.T @ hidden_errors * output_hidden * (1-output_hidden ))
        
        
        
    def fit(self,feature_train, label_train,epochs):
        # so it can handle dataframes
        feature_train = feature_train.tolist()
        label_train = label_train.tolist()
        
        for epoch in range(epochs):
        
            for i in range(len(feature_train)):
                target_vector = np.zeros(self.output_nodes)
                target_vector[int(label_train[i])] = 0.99
                self.train(input_list = feature_train[i],target_list=target_vector)
            
            if (epoch%10)==0:
                print("Epoch ",epoch)
        
        
    def predict(self,feature_test):
        # so it can handle dataframes
        feature_test = feature_test.tolist()
        
        return [np.argmax(self.feedforward(feature_test[i] )) for i in range(len(feature_test))]