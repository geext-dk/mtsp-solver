using System.Collections.Generic;
using System.Collections;
using System;

namespace mtsp
{
    // Класс решений. Каждый объект содержит конкретное решение
    // Каждое решение закодировано в массиве следующим образом:
    // Размер массива - number_of_shops + number_of_cars
    // Первые number_of_shops элементов - числа, расположенные в порядке обхода
    // После идут number_of_cars чисел. Эти числа указывают, сколько машин охватывает
    // каждая машина.
    // Например:
    //  ___ _______
    // [2 3 1 6 5 4 | 2 4], где number_of_shops = 6, а number_of_cars = 2
    // Где первая машина проходит два магазина - 2 и 3
    // А вторая машина проходит четыре магазина = 1, 6, 5 и 4
    public class Solution : IEnumerable<int[]>
    {
        private int[] shops;
        private int[] car_path_lengths;
        private int number_of_cars;
        private int number_of_shops;

        public Solution(int[] shops, int[] car_path_lengths)
        {
            this.car_path_lengths = new int[car_path_lengths.Length];
            this.shops = new int[shops.Length];
            Array.Copy(shops, this.shops, shops.Length);
            Array.Copy(car_path_lengths, this.car_path_lengths, car_path_lengths.Length);
            number_of_cars = car_path_lengths.Length;
            number_of_shops = shops.Length;
        }

        public Solution(int number_of_shops, int number_of_cars)
        {
            this.number_of_shops = number_of_shops;
            this.number_of_cars = number_of_cars;
            RegenerateCarPathLengths();
            RegenerateVisitedShops();
        }

        public int[] GetShops()
        {
            return shops;
        }

        public Solution Copy()
        {
            return new Solution(shops, car_path_lengths);
        }
        

        // поддержка foreach
        // При каждом возврате формирует массив магазинов, которые проходит
        // очередная машина
        public IEnumerator<int[]> GetEnumerator()
        {
            int next = 0;
            for(int car = 0; car < car_path_lengths.Length; ++car)
            {
                int shops_visited = car_path_lengths[car];
                int[] ret = new int[shops_visited];
                for (int j = 0; j < shops_visited; ++j)
                {
                    ret[j] = shops[next + j];
                }
                next += shops_visited;

                yield return ret;
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        public void RegenerateCarPathLengths()
        {
            Random rand = new Random();
            car_path_lengths = new int[number_of_cars];
            bool[] is_assigned = new bool[number_of_cars];

            int number_of_assigneg_shops = 0;
            for (int j = 0; j < number_of_cars - 1; ++j)
            {
                // сначала генерируем случайное количество магазинов
                int random_number = rand.Next(2, number_of_shops - 2 * (number_of_cars - j) - number_of_assigneg_shops + 2);
                // и присваиваем случайной машине, которая еще не присваивалась
                int random_position = Global.GetRandomFree(is_assigned, number_of_cars - j);
                car_path_lengths[random_position] = random_number;
                is_assigned[random_position] = true;
                number_of_assigneg_shops += random_number;
            }
            // последней машине просто отдаём все оставшиеся города
            int last_car;
            for (last_car = 0; last_car < number_of_cars; ++last_car)
            {
                if (!is_assigned[last_car])
                {
                    break;
                }
            }
            car_path_lengths[last_car] = number_of_shops - number_of_assigneg_shops;
        }

        public void RegenerateVisitedShops()
        {
            shops = new int[number_of_shops];
            bool[] is_assigned = new bool[number_of_shops];
            for (int position = 0; position < number_of_shops; ++position)
            {
                int random_shop = Global.GetRandomFree(is_assigned, number_of_shops - position);
                is_assigned[random_shop] = true;
                shops[position] = random_shop;
            }
        }

        public override string ToString()
        {
            string str = "";
            foreach(int shop in shops)
            {
                str += shop + " ";
            }
            str += "| ";
            foreach(int car_path in car_path_lengths)
            {
                str += car_path + " ";
            }
            return str;
        }

        public string GetGlobalString(List<int> global_shops)
        {
            string str = "";
            foreach(int shop in shops)
            {
                str += global_shops[shop] + " ";
            }
            str += "| ";
            foreach(int car_path in car_path_lengths)
            {
                str += car_path + " ";
            }
            return str;
        }
    }
}
