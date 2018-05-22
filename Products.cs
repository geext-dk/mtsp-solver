using System;
using System.Collections.Generic;
using System.IO;

namespace mtsp
{
    public class Products
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

            if (!ReadData("input.txt", out adjacency_list, out number_of_cars,
                     out storage, out shops))
            {
                return;
            }

            // Переводим в полный граф
            int[,] adjacency_matrix;
            int[] storage_distance;
            ConvertToCompleteGraph(adjacency_list, shops, storage, out adjacency_matrix,
                                   out storage_distance);


            // инициализируем решатель
            MtspSolver solver = new MtspSolver
                    (new Graph(adjacency_matrix), shops, storage, storage_distance, number_of_cars);

            solver.Solve();
        }


        // Функция, читающая данные из переданного файла
        public static bool ReadData(string input_file,
                out Dictionary<int, List<Edge>> adjacency_list,
                out int number_of_cars, out int storage, out List<int> shops)
        {
            try
            {
                if (!File.Exists(input_file))
                {
                    throw new FileNotFoundException(input_file);
                }
                adjacency_list = new Dictionary<int, List<Edge>>();
                shops = new List<int>();

                // Читаем входные данные
                
                using (StreamReader reader = new StreamReader(input_file))
                {
                    // Первая строка во входном файле:
                    // <кол-во пунктов = n> <кол-во машин = m> <склад>
                    string line = reader.ReadLine();
                    string[] bits = line.Split(' ');
                    number_of_cars = int.Parse(bits[1]);
                    storage = int.Parse(bits[2]);

                    // На следующей строке расположен список магазинов
                    line = reader.ReadLine();
                    bits = line.Split(' ');
                    foreach (string shop in bits)
                    {
                        if (shop.Length == 0)
                        {
                            continue;
                        }
                        shops.Add(int.Parse(shop));
                    }

                    // После этого идёт список дорог и их расстояния в формате:
                    // <пункт1> <пункт2> <расстояние>
                    // указаны номера пунктов, они начинаются с 1
                    line = reader.ReadLine();
                    while (line != null)
                    {
                        bits = line.Split(' ');
                        int first = int.Parse(bits[0]) - 1;
                        int second = int.Parse(bits[1]) - 1;
                        int cost = int.Parse(bits[2]);
                        Edge edge_first_second = new Edge(second, cost);
                        Edge edge_second_first = new Edge(first, cost);
                        if (adjacency_list.ContainsKey(first))
                        {
                            adjacency_list[first].Add(new Edge(second, cost));
                        }
                        else
                        {
                            adjacency_list.Add(first, new List<Edge>() {edge_first_second});
                        }

                        if (adjacency_list.ContainsKey(second))
                        {
                            adjacency_list[second].Add(edge_second_first);
                        }
                        else {
                            adjacency_list.Add(second, new List<Edge>() {edge_second_first});
                        }
                        line = reader.ReadLine();
                    }
                }
                return true;
            }
            catch (Exception e)
            {
                Console.WriteLine(e.StackTrace);
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
        // Предполагает, что каждая вершина смежна с какой-то другой!
        public static void ConvertToCompleteGraph(Dictionary<int, List<Edge>> adjacency_list,
                List<int> shops, int storage, out int[,] adjacency_matrix, out int[] storage_distance)
        {
            Console.Write("Converting graph... ");
            adjacency_matrix = new int[shops.Count, shops.Count];

            for(int shop = 0; shop < shops.Count; ++shop)
            {
                int[] shop_distance;
                int[] shop_previous;
                Global.Dijkstra(adjacency_list, shops[shop], out shop_distance, out shop_previous);

                // тут мы теперь имеем расстояние от магазина shop до всех остальных магазинов.
                // пихаем информацию в матрицу смежности
                for (int connected_shop = 0; connected_shop < shops.Count; ++connected_shop)
                {
                    adjacency_matrix[shop, connected_shop] = shop_distance[shops[connected_shop]];
                    adjacency_matrix[connected_shop, shop] = shop_distance[shops[connected_shop]];
                }
                adjacency_matrix[shop, shop] = int.MaxValue;
            }

            // заполним отдельно массив расстояний от склада до остальных вершин
            storage_distance = new int[shops.Count];
            int[] distance;
            int[] previous;
            Global.Dijkstra(adjacency_list, storage, out distance, out previous);
            for (int shop = 0; shop < shops.Count; ++shop)
            {
                storage_distance[shop] = distance[shops[shop]];
            }
            Console.WriteLine("done");
        }
    }
}
