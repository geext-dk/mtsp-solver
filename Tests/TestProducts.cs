using Xunit;
using mtsp;
using System.Collections.Generic;
using System.IO;

namespace Tests
{
    public class TestProducts
    {
        private readonly string input_file_;
        private readonly int number_of_cars_;
        private readonly Dictionary<int, List<Edge>> adjacency_list_;
        private readonly int storage_;
        private readonly List<int> shops_;

        public TestProducts()
        {
            input_file_ = "test_input.txt";
            adjacency_list_ = new Dictionary<int, List<Edge>>()
            {
                {0, new List<Edge> {new Edge(2, 5), new Edge(4, 3)}},
                {1, new List<Edge> {new Edge(2, 3)}},
                {2, new List<Edge> {new Edge(0, 5), new Edge(1, 3), new Edge(3, 2), new Edge(4, 4)}},
                {3, new List<Edge> {new Edge(2, 2), new Edge(4, 7)}},
                {4, new List<Edge> {new Edge(0, 3), new Edge(2, 4), new Edge(3, 7)}}
            };
            number_of_cars_ = 3;
            storage_ = 2;
            shops_ = new List<int> { 1, 4 };
        }

        [Fact]
        public void TestReadData()
        {
            // Сначала запишем данные в тестовый файл
            // Затем проверим, что прочла функция
            // И удалим файл
            using (StreamWriter writer = new StreamWriter(input_file_))
            {
                string data;
                data = adjacency_list_.Keys.Count.ToString() + " " + number_of_cars_ + " " + storage_;
                writer.WriteLine(data);
                data = "";
                foreach (int shop in shops_)
                {
                    data += shop.ToString() + " ";
                }
                writer.WriteLine(data);

                foreach (int shop in adjacency_list_.Keys)
                {
                    foreach (Edge adjacent_shop in adjacency_list_[shop])
                    {
                        data = shop.ToString() + " " + adjacent_shop.destination.ToString() +
                                " " + adjacent_shop.weight.ToString();
                        writer.WriteLine(data);
                    }
                }
            }

            Dictionary<int, List<Edge>> adjacency_list;
            int number_of_cars;
            int storage;
            List<int> shops;

            Products.ReadData(input_file_, out adjacency_list, out number_of_cars,
                    out storage, out shops);
            Assert.NotNull(adjacency_list);

            // Удаляем файл
            File.Delete(input_file_);

            // Проверяем данные
            Assert.Equal(adjacency_list_.Count, adjacency_list.Count);
            foreach (int shop in adjacency_list_.Keys)
            {
                Assert.Contains(shop, adjacency_list.Keys);
                foreach (Edge adjacent_shop in adjacency_list_[shop])
                {
                    Assert.Contains<Edge>(adjacent_shop, adjacency_list[shop]);
                }
            }

            foreach (int shop in adjacency_list.Keys)
            {
                Assert.Contains(shop, adjacency_list_.Keys);
                foreach (Edge adjacent_shop in adjacency_list[shop])
                {
                    Assert.Contains<Edge>(adjacent_shop, adjacency_list_[shop]);
                }
            }
        }
    }
}
