using System;
using System.Collections.Generic;

namespace mtsp
{
    // Класс уже оперирует над измененным (полным) графом
    public class MtspSolver
    {
        public static int InitialNumber = 20;
        public static int MaximumMutationNumber = InitialNumber / 4;

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
            for (int i = 0; i < times; ++i)
            {
                Crossover();
                int mutations = rand.Next() % MaximumMutationNumber;
                for (int j = 0; j < mutations; j++)
                {
                    int chromosome = rand.Next() % population.Count;
                    Mutation(population[chromosome], rand.Next());
                }
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
        private void Mutation(Solution solution, int type)
        {
            // Первый тип: меняем местами случайные элементы среди магазинов
            if (type % 2 == 1)
            {
                int[] shops = solution.GetShops();
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
                solution.RegenerateCarPathLengths();
            }
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
    }
}
