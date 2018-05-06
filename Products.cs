using System;
using System.Collections.Generic;
using System.IO;
using Medallion.Collections;

namespace mtsp
{
    class Products
    {
        //
        // Запуск
        //
        static void Main(string[] args)
        {
            // ДАНО:
            // Город в виде графа... (список смежности)
            Dictionary<int, List<Edge>> adjacency_list;
            int number_of_cars;      // Количество задействованных машин
            int storage;             // склад
            List<int> shops;         // Список магазинов


            ReadData("input.txt", out adjacency_list, out number_of_cars,
                     out storage, out shops);

            // добавим склад в массив магазинов для удобства
            shops.Add(storage);

            // Переводим в полный граф
            int[,] adjacency_matrix;
            ConvertToCompleteGraph(adjacency_list, shops, out adjacency_matrix);

            // инициализируем решатель
            MtspSolver solver = new MtspSolver(adjacency_matrix, storage, number_of_cars);

            // решаем
            solver.Solve(1000);
        }


        // Функция, читающие данные из переданного файла
        static bool ReadData(string input_file, out Dictionary<int, List<Edge>> adjacency_list,
                out int number_of_cars, out int storage, out List<int> shops)
        {
            try
            {
                adjacency_list = new Dictionary<int, List<Edge>>();
                shops = new List<int>();

                // Читаем входные данные
                using (TextReader reader = File.OpenText(input_file))
                {
                    // Первая строка во входном файле:
                    // <кол-во пунктов = n> <кол-во машин = m> <склад>
                    string line = reader.ReadLine();
                    string[] bits = line.Split(' ');
                    number_of_cars = int.Parse(bits[1]);
                    storage = int.Parse(bits[3]);

                    // На следующей строке расположен список магазинов
                    line = reader.ReadLine();
                    bits = line.Split(' ');
                    foreach (string shop in bits)
                    {
                        shops.Add(int.Parse(shop));
                    }

                    // После этого идёт список дорог и их расстояния в формате:
                    // <пункт1> <пункт2> <расстояние>
                    line = reader.ReadLine();
                    while (line != null)
                    {
                        bits = line.Split(' ');
                        int first = int.Parse(bits[0]);
                        int second = int.Parse(bits[1]);
                        int cost = int.Parse(bits[2]);
                        adjacency_list[first].Add(new Edge(second, cost));
                        adjacency_list[second].Add(new Edge(first, cost));
                        line = reader.ReadLine();
                    }
                }
                return true;
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
                Console.WriteLine("Error during reading input file: " + input_file);
                number_of_cars = 0;
                storage = 0;
                adjacency_list = null;
                shops = null;
                return false;
            }
        }


        // Функция, переводящая наш граф в полный, содержащий только магазины и склад
        // Предполагает, что каждая вершина смежна с какой-то другой! (то есть граф связный)
        static void ConvertToCompleteGraph(Dictionary<int, List<Edge>> adjacency_list,
                List<int> shops, out int[,] adjacency_matrix)
        {
            adjacency_matrix = new int[shops.Count, shops.Count];

            foreach (int shop in shops)
            {
                // реализация алгоритма Дейкстры
                int[] distance = new int[adjacency_list.Keys.Count];
                int[] previous = new int[adjacency_list.Keys.Count];
                PriorityQueue<int> priority_queue = new PriorityQueue<int>();
                foreach (int vertex in adjacency_list.Keys)
                {
                    priority_queue.Enqueue(vertex);
                    distance[vertex] = -1;
                    previous[vertex] = -1;
                }
                distance[shop] = 0;

                while (priority_queue.Count != 0)
                {
                    int u = priority_queue.Dequeue();
                    foreach (Edge v in adjacency_list[u])
                    {
                        int alt = distance[u] + v.weight;
                        if (alt < distance[v.destination] || distance[v.destination] == -1)
                        {
                            distance[v.destination] = alt;
                            previous[v.destination] = u;
                        }
                    }
                }
                // конец работы алгоритма Дейкстры

                // тут мы теперь имеем расстояние от магазина shop до всех остальных магазинов.
                // пихаем информацию в матрицу смежности
                foreach (int connected_shop in shops)
                {
                    adjacency_matrix[shop, connected_shop] = distance[connected_shop];
                    adjacency_matrix[connected_shop, shop] = distance[connected_shop];
                }
            }
        }
    }
}
