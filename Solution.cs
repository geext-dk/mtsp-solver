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
        private int[] solution_;
        private int number_of_cars_;

        public Solution(int[] data, int number_of_cars)
        {
            solution_ = data;
            number_of_cars_ = number_of_cars;
        }

        public Solution(int[] shops, int[] car_path_lengths)
        {
            solution_ = new int[shops.Length + car_path_lengths.Length];
            Array.Copy(shops, solution_, shops.Length);
            Array.Copy(car_path_lengths, 0, solution_, shops.Length, car_path_lengths.Length);
            number_of_cars_ = car_path_lengths.Length;
        }

        public int GetNumberOfCars()
        {
            return number_of_cars_;
        }
        
        public int GetCarPathLength(int car_number)
        {
            return solution_[solution_.Length - number_of_cars_ + car_number];
        }


        // поддержка foreach
        // При каждом возврате формирует массив магазинов, которые проходит
        // очередная машина
        public IEnumerator<int[]> GetEnumerator()
        {
            int next = 0;
            for(int car = solution_.Length - number_of_cars_; car < solution_.Length; ++car)
            {
                int shops_visited = solution_[car];
                int[] ret = new int[shops_visited];
                for (int j = 0; j < shops_visited; ++j)
                {
                    ret[j] = solution_[next + j];
                }
                next += shops_visited;

                yield return ret;
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
}
