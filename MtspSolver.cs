using System;
using System.Collections.Generic;

namespace mtsp
{
    // Класс уже оперирует над измененным (полным) графом
    public class MtspSolver
    {
        private int[,] adjacency_matrix_;
        private int number_of_cars_;
        private int storage_;
        private List<Solution> population_;


        // конструктор
        public MtspSolver(int[,] adjacency_matrix, int storage, int number_of_cars)
        {
            adjacency_matrix_ = adjacency_matrix;
            number_of_cars_ = number_of_cars;
            storage_ = storage;
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
