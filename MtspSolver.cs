using System;
using System.Collections.Generic;

namespace mtsp
{
    // Класс уже оперирует над измененным (полным) графом
    public class MtspSolver
    {
        private int[,] adjacency_matrix;
        private int number_of_cars;
        private int storage;

        // Список, состоящий из решений
        private List<Solution> population;


        // конструктор
        public MtspSolver(int[,] adjacency_matrix, int storage, int number_of_cars)
        {
            this.adjacency_matrix = adjacency_matrix;
            this.number_of_cars = number_of_cars;
            this.storage = storage;
        }

        // Запускаемая функция для решения задачи. Выполняется times раз
        public void Solve(int times)
        {
            // Каждое решение - это 
            // генерируем начальные решения
            GenerateInitialPopulation();

            for (int i = 0; i < times; ++i)
            {
                CrossingOver();
                Mutation();
                Selection();
            }
            // как-то выводим получившееся решение
        }


        // Целевая функция
        // Решение - это список (длиную в number_of_cars)
        // каждый элемент - отдельный список вершин, которые проходит каждая отдельная машина
        // ищет максимальный путь среди всех машин (который нам надо уменьшить)
        private int TargetFunction(Solution solution)
        {
            int max_distance = 0;
            foreach (List<int> car_path in solution)
            {
                int previous_shop = car_path[0];
                int distance = 0;
                foreach (int shop in car_path)
                {
                    distance += adjacency_matrix[previous_shop, shop];
                    previous_shop = shop;
                }
                if (max_distance < distance)
                {
                    max_distance = distance;
                }
            }

            return max_distance;
        }

        // генерируем начальную популяцию
        // тут заполняем поле population определенными решениями
        private void GenerateInitialPopulation()
        {

        }

        // Мутация 
        // Изменяем каким-то образом решения из population
        private void Mutation()
        {

        }

        // Скрещивание
        // Скрещиваем какие-то решения из population
        private void CrossingOver()
        {

        }

        // Селекция
        // Отбираем каким-то образом решения
        private void Selection()
        {

        }
    }
}
