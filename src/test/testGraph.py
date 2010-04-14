'''
Created on 08-Apr-2010

@author: vjethava
'''

import dai 
import numpy 
from interface.ArffReader import ArffReader
from algorithm.chowliu.ChowLiu import ChowLiu
from interface.QueryReader import QueryReader
from data.dataset import Dataset
import sys 
#if __name__=="__main__":
arff_file = '/home/vjethava/CODE/webintent/QueryLabels.arff'
query_file =  '/home/vjethava/webintent/Matrix'
reader = ArffReader(arff_file)
querier = QueryReader(query_file)
queries = querier.get_queries()
attributes = reader.getAttributes()
instances = reader.getInstances()   
dataset = Dataset(attributes, instances, queries)
cl_solver = ChowLiu()
mutual_info = cl_solver.getMI(attributes, instances)
edges = cl_solver.getListOfEdges()
# initialize the graph. 
facet_vars = [] 
vcount = 0
my_graph = dai.FactorGraph()
my_factors = dai.VecFactor()
for attribute in attributes:
    vcount = vcount + 1 
    clabel =vcount
    cstates = attribute.getNumVals()
    cvar = dai.Var(clabel, cstates)
    facet_vars.append(cvar)
print edges
# make the factors corresponding to chow liu
edge_idx = 0 
edge_probs = [] 
for edge in edges: 
    ex = int(edge[0])
    ey = int(edge[1])
    print "edge",  str(edge) , str(attributes[ex]), str(attributes[ey])
    prob_edge = cl_solver.get_prob_for_edge(edge_idx)
    norm_prob_edge = 1.0/numpy.size(prob_edge) + prob_edge/2 
    print "prob sum: ", numpy.sum(norm_prob_edge), " matrix: ", norm_prob_edge
    edge_idx = edge_idx + 1  
    edge_probs.append(norm_prob_edge)
    # generate the factors appropriately
    cfactor = dai.Factor(facet_vars[ex], facet_vars[ey])
    # fill the factor probability distribution 
    eshape = norm_prob_edge.shape 
    x_var_num = attributes[ex].numVals
    y_var_num = attributes[ey].numVals
    print "x num: ", x_var_num, "y num: ", y_var_num 
    for i in range(norm_prob_edge.size):
        yi = int(i/x_var_num)
        xi = int(i%x_var_num) 
        cfactor[i] = norm_prob_edge[xi, yi]
    my_factors.append(cfactor)
        








        