using System;

// graph with adjacency matrix
public class Graph
{
    private readonly int[,] adjacency_matrix;
    private readonly int number_of_shops;
    public Graph(int[,] adjacency_matrix)
    {
        this.adjacency_matrix = adjacency_matrix;
        this.number_of_shops = adjacency_matrix.GetLength(0);
    }

    public int GetNumberOfShops()
    {
        return number_of_shops;
    }

    public int GetWeight(int first_vertex, int second_vertex)
    {
        return adjacency_matrix[first_vertex, second_vertex];
    }
}