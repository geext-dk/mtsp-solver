using System;
using System.Collections.Generic;

namespace mtsp
{
    // Класс уже оперирует над измененным (полным) графом
    public class MtspSolver
    {
        public static int InitialNumber = 8;
        public static int CrossoverNumber = InitialNumber;
        public static int MutationNumber = InitialNumber;

        private int[,] adjacency_matrix;
        private int number_of_cars;
        private int number_of_shops;
        private int storage;
        private List<Solution> population;
        Random rand;

        // конструктор
        public MtspSolver(int[,] adjacency_matrix, int storage, int number_of_cars)
        {
            rand = new Random();
            this.adjacency_matrix = adjacency_matrix;
            this.number_of_cars = number_of_cars;
            this.number_of_shops = this.adjacency_matrix.GetLength(0);
            this.storage = storage;
            population = new List<Solution>();
        }

        // Запускаемая функция для решения задачи. Выполняется times раз
        public void Solve(int times)
        {
            // Каждое решение - это 
            // генерируем начальные решения
            GenerateInitialPopulation();

            // алгоритм
            for (int i = 0; i < times; ++i)
            {
                // Скрещивание
                for (int j = 0; j < CrossoverNumber; ++j)
                {
                    int first_chromosome = rand.Next() % population.Count;
                    int second_chromosome;
                    do
                    {
                        second_chromosome = rand.Next() % population.Count;
                    } while (first_chromosome == second_chromosome);
                    Solution child = Crossover(population[first_chromosome], population[second_chromosome]);
                    population.Add(child);
                }

                // Мутация
                for (int j = 0; j < MutationNumber; ++j)
                {
                    int chromosome = rand.Next() % population.Count;
                    population.Add(Mutation(population[chromosome], rand.Next()));
                }

                // Селекция
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
                int previous_shop = this.storage;
                foreach (int shop in car_path)
                {
                    distance += this.adjacency_matrix[previous_shop, shop];
                    previous_shop = shop;
                }
                distance += this.adjacency_matrix[previous_shop, this.storage];
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
                this.population.Add(new Solution(number_of_shops, number_of_cars));
            }
        }

        // Мутация 
        // Изменяем каким-то образом решения из population
        private Solution Mutation(Solution solution, int type)
        {
            Solution new_solution = solution.Copy();
            // Первый тип: меняем местами случайные элементы среди магазинов
            if (type % 2 == 1)
            {
                int[] shops = new_solution.GetShops();
                int first = rand.Next() % number_of_shops;
                int second;
                do
                {
                    second = rand.Next() % number_of_shops;
                }
                while (first == second);
                int x = shops[first];
                shops[first] = shops[second];
                shops[second] = x;
            }
            // Второй тип: перегенерируем количество магазинов, которые проезжает каждая машина
            else
            {
                new_solution.RegenerateCarPathLengths();
            }
            return new_solution;
        }

        // Скрещивание
        // Скрещиваем какие-то решения из population
        private Solution Crossover(Solution first, Solution second)
        {
            int[] first_shops = first.GetShops();
            int[] second_shops = second.GetShops();


            Solution new_first = first.Copy();
            int[] new_first_shops = first.GetShops();
            bool[] new_first_is_assigned = new bool[number_of_shops];
            bool[] new_first_is_shop_assigned = new bool[number_of_shops];

            // Рандомно получаем последовательность из первого родителя и переносим ребёнку
            int length = rand.Next() % (number_of_shops - 2) + 2;
            int start_with = rand.Next() % (number_of_shops - length);
            for (int i = start_with; i < length + start_with; ++i)
            {
                new_first_is_assigned[i] = true;
                new_first_is_shop_assigned[first_shops[i]] = true;
            }
            // Оставшиеся магазины переносим в ребёнка из второго родителя
            for (int i = 0; i < number_of_shops; ++i)
            {
                // находим первую необработанную ячейку
                if (new_first_is_assigned[i])
                {
                    continue;
                }
                // и присваиваем первый не присвоенный магазин
                for (int j = 0; j < number_of_shops; ++j)
                {
                    if (!new_first_is_shop_assigned[j])
                    {
                        new_first_shops[i] = j;
                        new_first_is_assigned[i] = true;
                        new_first_is_shop_assigned[j] = true;
                    }

                }
            }
            return new_first;
        }

        // Селекция
        // Отбираем каким-то образом решения
        private void Selection()
        {

        }
    }
}
