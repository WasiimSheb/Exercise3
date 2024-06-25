import random

def generate_graph_file(num_vertices, num_edges, filename):
    with open(filename, 'w') as f:
        f.write(f"{num_vertices} {num_edges}\n")
        edges = set()
        while len(edges) < num_edges:
            u = random.randint(1, num_vertices)
            v = random.randint(1, num_vertices)
            if u != v:
                edge = (min(u, v), max(u, v))
                edges.add(edge)
        
        for edge in edges:
            f.write(f"{edge[0]} {edge[1]}\n")

num_vertices = 10000  # Change this to the desired number of vertices
num_edges = 10000     # Change this to the desired number of edges
filename = "graph.txt"

generate_graph_file(num_vertices, num_edges, filename)
print(f"Graph file '{filename}' generated with {num_vertices} vertices and {num_edges} edges.")
