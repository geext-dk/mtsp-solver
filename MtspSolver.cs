using System;
using System.Collections.Generic;

namespace mtsp
{
    // Класс уже оперирует над измененным (полным) графом
    public class MtspSolver
    {
        public static int InitialNumber = 20;
        public static int MaximumMutationNumber = InitialNumber / 2;

        private int[,] adjacency_matrix_;
        private int number_of_cars_;
        private int number_of_shops_;
        private int storage_;
        private List<Solution> population_;
        Random rnd_;

        // конструктор
        public MtspSolver(int[,] adjacency_matrix, int storage, int number_of_cars)
        {
            adjacency_matrix_ = adjacency_matrix;
            number_of_cars_ = number_of_cars;
            number_of_shops_ = adjacency_matrix_.GetLength(0);
            storage_ = storage;
            rnd_ = new Random();
            population_ = new List<Solution>();
        }

        // Запускаемая функция для решения задачи. Выполняется times раз
        public void Solve(int times)
        {
            // Каждое решение - это 
            // генерируем начальные решения
            GenerateInitialPopulation();

            for (int i = 0; i < times; ++i)
            {
                Crossover();
                Mutation();
                Selection();
            }
            // как-то выводим получившееся решение
        }


        // Целевая функция
        // ищет максимальный путь среди всех машин (который нам надо уменьшить)
        private int ObjectiveFunction(Solution solution)
        {
            int max_distance = 0;
            foreach (int[] car_path in solution)
            {
                int distance = 0;
                int previous_shop = storage_;
                foreach (int shop in car_path)
                {
                    distance += adjacency_matrix_[previous_shop, shop];
                    previous_shop = shop;
                }
                distance += adjacency_matrix_[previous_shop, storage_];
                if (distance > max_distance)
                {
                    max_distance = distance;
                }
            }
            return max_distance;
        }

        // генерируем начальную популяцию
        // тут заполняем поле population случайными особями
        private void GenerateInitialPopulation()
        {
            for (int i = 0; i < InitialNumber; ++i)
            {
                int[] visited_shops = GenerateVisitedShops();
                int[] car_path_lengths = GenerateCarPathLengths();
                population_.Add(new Solution(visited_shops, car_path_lengths));
            }
        }

        // Мутация 
        // Изменяем каким-то образом решения из population
        private void Mutation()
        {

        }

        // Скрещивание
        // Скрещиваем какие-то решения из population
        private void Crossover()
        {

        }

        // Селекция
        // Отбираем каким-то образом решения
        private void Selection()
        {

        }


        private int[] GenerateCarPathLengths()
        {
            int[] car_path_lengths = new int[number_of_cars_];
            bool[] is_assigned = new bool[number_of_cars_];

            int number_of_assigneg_shops = 0;
            for (int j = 0; j < number_of_cars_ - 1; ++j)
            {
                // сначала генерируем число
                int random_number = rnd_.Next()
                        % (number_of_shops_ - (number_of_cars_ - j) - number_of_assigneg_shops + 1) + 1;
                // и присваиваем случайной машине, которая еще не присваивалась
                int random_position = GetRandomFree(is_assigned, number_of_cars_ - j);
                car_path_lengths[random_position] = random_number;
                is_assigned[random_position] = true;
                number_of_assigneg_shops += random_number;
            }
            // последней машине просто отдаём все оставшиеся города
            int last_car;
            for (last_car = 0; last_car < number_of_cars_; ++last_car)
            {
                if (!is_assigned[last_car])
                {
                    break;
                }
            }
            car_path_lengths[last_car] = number_of_shops_ - number_of_assigneg_shops;
            return car_path_lengths;
        }

        private int[] GenerateVisitedShops()
        {
            int[] visited_shops = new int[number_of_shops_];
            bool[] is_assigned = new bool[number_of_shops_];
            for (int position = 0; position < number_of_shops_; ++position)
            {
                int random_shop = GetRandomFree(is_assigned, number_of_shops_ - position);
                is_assigned[random_shop] = true;
                visited_shops[position] = random_shop;
            }
            return visited_shops;
        }

        private int GetRandomFree(bool[] is_assigned, int number_of_free)
        {
            int random_position = rnd_.Next() % number_of_free;
            int k = -1;
            do
            {
                ++k;
                if (is_assigned[k])
                {
                    ++random_position;
                }
            }
            while (k != random_position);
            return random_position;
        }
    }
}
