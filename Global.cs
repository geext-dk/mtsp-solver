using System;
using Priority_Queue;
using System.Collections.Generic;

namespace mtsp
{
    public class Global
    {
        public static int GetRandomFree(bool[] is_assigned, int number_of_free)
        {
            Random rand = new Random();
            int random_position = rand.Next(0, number_of_free);
            int k = -1;
            do
            {
                ++k;
                if (is_assigned[k])
                {
                    ++random_position;
                }
            }
            while (k < random_position);
            return random_position;
        }

        public static void Dijkstra(Dictionary<int, List<Edge>> adjacency_list, int from,
                    out int[] distance, out int[] previous)
        {
            SimplePriorityQueue<int> priority_queue = new SimplePriorityQueue<int>();
            distance = new int[adjacency_list.Keys.Count];
            previous = new int[adjacency_list.Keys.Count];
            foreach (int vertex in adjacency_list.Keys)
            {
                previous[vertex] = -1;
                distance[vertex] = int.MaxValue;
                priority_queue.Enqueue(vertex, int.MaxValue);
            }
            priority_queue.UpdatePriority(from, 0);
            distance[from] = 0;

            while (priority_queue.Count != 0)
            {
                int u = priority_queue.Dequeue();
                foreach (Edge v in adjacency_list[u])
                {
                    int alt = distance[u] + v.weight;
                    if (alt < distance[v.destination] || distance[v.destination] == -1)
                    {
                        distance[v.destination] = alt;
                        priority_queue.UpdatePriority(v.destination, alt);
                        previous[v.destination] = u;
                    }
                }
            }
        }
    }

}
