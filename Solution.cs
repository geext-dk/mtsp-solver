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
    
using System.Collections.Generic;
using System.Collections;
using System;

namespace mtsp
{
    public class Solution : IEnumerable<int[]>
    {
        private int[] shops_array;
        private int[] path_lengths;
        private int number_of_cars;
        private int number_of_shops;

        public Solution(int[] shops_array, int[] path_lengths)
        {
            number_of_shops = shops_array.Length;
            number_of_cars = path_lengths.Length;

            this.path_lengths = new int[number_of_cars];
            this.shops_array = new int[number_of_shops];
            Array.Copy(shops_array, this.shops_array, shops_array.Length);
            Array.Copy(path_lengths, this.path_lengths, path_lengths.Length);
        }

        public Solution(int number_of_shops, int number_of_cars)
        {
            this.number_of_shops = number_of_shops;
            this.number_of_cars = number_of_cars;
            RegenerateCarPathLengths();
            RegenerateVisitedShops();
        }

        public int[] GetShopsArray()
        {
            return shops_array;
        }

        public int[] GetPathLengths()
        {
            return path_lengths;
        }

        public Solution Copy()
        {
            return new Solution(shops_array, path_lengths);
        }
        

        // foreach support
        public IEnumerator<int[]> GetEnumerator()
        {
            int next = 0;
            for(int car = 0; car < path_lengths.Length; ++car) {
                int shops_visited = path_lengths[car];
                int[] ret = new int[shops_visited];
                for (int j = 0; j < shops_visited; ++j) {
                    ret[j] = shops_array[next + j];
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
            path_lengths = new int[number_of_cars];
            bool[] is_assigned = new bool[number_of_cars];

            int number_of_assigned_shops = 0;
            for (int j = 0; j < number_of_cars - 1; ++j) {
                int random_number_of_shops = rand.Next(2,
                        number_of_shops - 2 * (number_of_cars - j) - number_of_assigned_shops + 2);
                int random_car = Global.GetRandomFree(is_assigned, number_of_cars - j);
                path_lengths[random_car] = random_number_of_shops;
                is_assigned[random_car] = true;
                number_of_assigned_shops += random_number_of_shops;
            }
            
            int last_car;
            for (last_car = 0; last_car < number_of_cars; ++last_car)
            {
                if (!is_assigned[last_car])
                {
                    break;
                }
            }
            path_lengths[last_car] = number_of_shops - number_of_assigned_shops;
        }

        public void RegenerateVisitedShops()
        {
            shops_array = new int[number_of_shops];
            bool[] is_assigned = new bool[number_of_shops];
            for (int position = 0; position < number_of_shops; ++position)
            {
                int random_shop = Global.GetRandomFree(is_assigned, number_of_shops - position);
                is_assigned[random_shop] = true;
                shops_array[position] = random_shop;
            }
        }

        public override string ToString()
        {
            string str = "";
            foreach(int shop in shops_array)
            {
                str += shop + " ";
            }
            str += "| ";
            foreach(int car_path in path_lengths)
            {
                str += car_path + " ";
            }
            return str;
        }

        public string GetGlobalString(int[] global_shops)
        {
            string str = "";
            foreach(int shop in shops_array)
            {
                str += global_shops[shop] + " ";
            }
            str += "| ";
            foreach(int car_path in path_lengths)
            {
                str += car_path + " ";
            }
            return str;
        }
    }
}
