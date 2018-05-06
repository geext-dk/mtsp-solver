namespace mtsp
{
    public struct Edge
    {
        public int destination;
        public int weight;
        public Edge(int d, int w)
        {
            destination = d;
            weight = w;
        }
    }
}