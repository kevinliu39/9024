# g = Graph()
# for i in range(6):
#   g.addVertex(i)
# print g.vertList


class Vertex:
  def __init__(self,key):
    self.connectedV = {}
    self.val = key
  
  def addNeighbor(self,neighbor, weight=0):
    self.connectedV[neighbor] = weight

  def __str__(self):
    return str(self.val)+' connected to '+ str([x for x in self.connectedV])

  def getConnections(self):
    return self.connectedV.keys()
    
  def getVal(self):
    return self.val

  def getWeight(self,nbr):
    return self.connectedV[nbr]


class Graph:
  def __init__(self):
    self.numV = 0
    self.vertexList = {}

  def addVertex(self, key):
    self.numV += 1
    newV = Vertex(key)
    self.vertexList[key] = newV

  def getVertex(self, key):
    if key in self.vertexList:
      return self.vertexList[key]
    else:
      return None

  def __contains__(self, key):
    return key in self.vertexList

  def addEdge(self, startV, toV, cost=0):
    if startV not in self.vertexList:
      self.addVertex(startV)
    if toV not in self.vertexList:
      self.addVertex(toV)
    self.vertexList[startV].addNeighbor(toV,cost)
    self.vertexList[toV].addNeighbor(startV,cost)

  def getVertices(self):
    return self.vertexList.keys()

  def __iter__(self):
    return iter(self.vertexList.values())
  #for V in Graph:

if __name__ == "__main__":
  print('Graph Start!')
  g = Graph()
  g.addEdge(0,1,2)
  g.addEdge(2,3,2)
  g.addEdge(2,1,3)
  for i in g.vertexList:
    print(g.vertexList[i])
  for j in g:
    print(j)

