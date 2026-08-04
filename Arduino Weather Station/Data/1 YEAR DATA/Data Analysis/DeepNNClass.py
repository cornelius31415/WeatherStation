#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon May 20 06:17:57 2024

@author: cornelius
"""

"""
                       DEEP NEURAL NETWORK CLASS
            
            -> Type of Neural Network:  Feed Forward
            -> Activation Function:     Sigmoid
            -> Optimizer:               Gradient Descent
                

            with this class it is possible to construct simple
            feed forward neural networks with multiple layers.
            A layer is simply added by calling the neural network
            object and applying the layer method.

"""

# -----------------------------------------------------------------------------
#                               IMPORTS
# -----------------------------------------------------------------------------

import numpy as np
import scipy.special


# -----------------------------------------------------------------------------
#                           CLASS DEFINITION
# -----------------------------------------------------------------------------

class NeuralNetwork():
    
    def __init__(self,input_nodes,learning_rate):
        
        self.weight_list = []           # each element is  a matrix of weights
        
        self.learning_rate = learning_rate # learning rate of the neural net
       
        self.node_list = [input_nodes]  # each element is the amount of nodes for that layer
        
        
    # CREATE A NEW LAYER
    
    def layer(self,output_nodes):
        
        input_nodes = self.node_list[-1]                        
        weight = np.random.rand(input_nodes,output_nodes)-0.5 
        self.weight_list.append(weight)
        self.node_list.append(output_nodes)
        
      
    # TRAINING THE NETWORK

    def train(self,input_list, target_list):
        input_list = np.array(input_list,ndmin=2)
        target_list = np.array(target_list,ndmin=2)
        
        outputs = [input_list]
        
        # FEED FORWARD PART
        # transfer the input signal all the way through the network
        # to the output layer
        
        for weight in self.weight_list:
            net_inputs = outputs[-1] @ weight
            outputs.append(scipy.special.expit(net_inputs))
            

        # ERROR PROPAGATION
        # propagate the errors at the output layer nodes back to the weights
        
        errors = [target_list - outputs[-1]]
        
        for i in range(len(self.weight_list)-1,0,-1):
            errors.append(errors[-1] @ self.weight_list[i].T)
        
        errors.reverse()

    
        # WEIGHT ADJUSTMENT
        # adjust all of the weight matrices with gradient descent
        
        for i in range(len(self.weight_list)):
            gradient = errors[i]*outputs[i+1]*(1-outputs[i+1])
            self.weight_list[i] += self.learning_rate * (outputs[i].T @ gradient)
        

        
    # FITTING THE NETWORK TO TRAINING DATA
    # this is inspired by the sklearn style
    # type(features) = numpy array of arrays
    # type(labels) = numpy array
    # type(epochs) = integer


    def fit(self,features,labels,epochs):
        features = features.tolist()
        labels = labels.tolist()
        for epoch in range(epochs):
            for i in range(len(features)):
                targetvector = np.zeros(self.node_list[-1]) + 0.01
                targetvector[int(labels[i])]=0.99
                self.train(input_list=features[i],target_list=targetvector)
    
                if (i%len(features))==0:
                    print(f"-----------    Epoch {epoch+1}    -----------")   
                    print()
                    
                    
                    
    # PREDICTING 
    # this also is inspired by the sklearn style
    # this also is inspired by the sklearn style
    # type(test_features) = numpy array of arrays

    def predict(self,test_features):
        # so it can handle dataframes
        test_features = test_features.tolist()
        results = []
        for features in test_features:
            inputs = np.array(features,ndmin=2)
            for weight in self.weight_list:
                inputs = scipy.special.expit(inputs @ weight)
            results.append(np.argmax(inputs))
        return results
  
    
  

    
