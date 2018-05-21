using System;
using System.Collections.Generic;

namespace mtsp
{
    // Класс уже оперирует над измененным (полным) графом
    public class MtspSolver
    {
        public static int MaximumSameResults = 10;
        public static int InitialNumber = 8;
        public static int CrossoverNumber = InitialNumber;  // т. е. + InitialNumber особей
        public static int MutationNumber = InitialNumber * 2;  // т. е. + еще InitialNumber особей (итого 3 * InitialNumber)
        // следовательно для получения изначально числа, нужно оставить 2/3 особей
        public static int SelectionGroupSize =
                (InitialNumber + CrossoverNumber + MutationNumber) / InitialNumber;

        private readonly int[,] adjacency_matrix;
        private readonly int number_of_cars;
        private readonly int number_of_shops;
        private List<Solution> population;
        private readonly List<int> global_shops;
        private readonly int[] storage_distance;
        private readonly Random rand;

        // конструктор
        // Мы преобразовываем глобальное решение в локальное (т. е. индексы из изначально графа
        // преобразуются в индексы в полного графа
        public MtspSolver(int[,] adjacency_matrix, List<int> global_shops, int storage,
                int[] storage_distance, int number_of_cars)
        {
            rand = new Random();
            population = new List<Solution>();

            this.adjacency_matrix = adjacency_matrix;
            this.global_shops = global_shops;
            this.number_of_cars = number_of_cars;
            this.storage_distance = storage_distance;
            number_of_shops = adjacency_matrix.GetLength(0);
        }

        // Запускаемая функция для решения задачи. Выполняется times раз
        public void Solve()
        {
            int cycles = 0;
            // генерируем начальные решения
            GenerateInitialPopulation();
            // алгоритм
            int i = 0;
            int last = 0; // последний результат функции
            Solution best_solution = new Solution(number_of_shops, number_of_cars);
            int function_result = 0;
            while (i < MaximumSameResults) {
                // Мутация
                for (int j = 0; j < MutationNumber; ++j) {
                    int chromosome = rand.Next(0, population.Count);
                    population.Add(Mutation(population[chromosome], rand.Next()));
                }

                // Скрещивание
                if (number_of_shops > 2) {
                    for (int j = 0; j < CrossoverNumber; ++j) {
                        int first_chromosome = rand.Next(0, population.Count);
                        int second_chromosome;
                        do {
                            second_chromosome = rand.Next(0, population.Count);
                        } while (first_chromosome == second_chromosome);
                        Solution child = Crossover(population[first_chromosome],
                                                   population[second_chromosome]);
                        population.Add(child);
                    }
                }

                // Селекция
                Selection(out best_solution, out function_result);

                if (last != function_result) {
                    cycles += i;
                    last = function_result;
                    i = 0;
                } else {
                    ++i;
                }
            }
            cycles += i;
            // Выводим
            Console.WriteLine(function_result + ": " + best_solution.GetGlobalString(global_shops));
            Console.WriteLine("Cycles: " + cycles);

        }


        // Целевая функция
        // ищет максимальный путь среди всех машин (который нам надо уменьшить)
        private int ObjectiveFunction(Solution solution)
        {
            int max_distance = 0;
            foreach (int[] car_path in solution)
            {
                int distance = storage_distance[car_path[0]];
                int previous_shop = car_path[0];
                for (int i = 1; i < car_path.Length; ++i)
                {
                    distance += adjacency_matrix[previous_shop, car_path[i]];
                    previous_shop = car_path[i];
                }
                distance += storage_distance[previous_shop];
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

        // Скрещивание
        // Скрещиваем какие-то решения из population
        private Solution Crossover(Solution first, Solution second)
        {
            int[] second_shops = second.GetShops();


            Solution child = first.Copy();
            // Если количество магазинов меньше 2, то скрещивать-то и нечего
            if (number_of_shops > 2)
            {
                int[] child_shops = child.GetShops();
                bool[] is_assigned = new bool[number_of_shops];
                bool[] is_shop_assigned = new bool[number_of_shops];

                // Рандомно получаем последовательность из первого родителя и переносим ребёнку
                int length = rand.Next(1, number_of_shops - 1);
                int start_with = rand.Next(1, number_of_shops - length);
                for (int i = start_with; i < length + start_with; ++i)
                {
                    is_assigned[i] = true;
                    is_shop_assigned[child_shops[i]] = true;
                }

                // Оставшиеся магазины переносим в ребёнка из второго родителя
                for (int i = 0; i < number_of_shops; ++i)
                {
                    // находим первую необработанную ячейку
                    if (is_assigned[i])
                    {
                        continue;
                    }
                    // и присваиваем первый не присвоенный магазин
                    foreach (int second_shop in second_shops)
                    {
                        if (!is_shop_assigned[second_shop])
                        {
                            child_shops[i] = second_shop;
                            is_assigned[i] = true;
                            is_shop_assigned[second_shop] = true;
                            break;
                        }
                    }
                }
            }
            return child;
        }

        // Мутация 
        // Изменяем каким-то образом решения из population
        private Solution Mutation(Solution solution, int type)
        {
            Solution new_solution = solution.Copy();
            // Первый тип: меняем местами случайные элементы среди магазинов
            int chance = type % 10;
            if (chance == 0 || chance == 1 || chance == 2)
            {
                int[] shops = new_solution.GetShops();
                int first = rand.Next(0, number_of_shops);
                int second;
                do
                {
                    second = rand.Next(0, number_of_shops);
                }
                while (first == second);
                int x = shops[first];
                shops[first] = shops[second];
                shops[second] = x;
            }
            // Второй тип: перегенерируем количество магазинов, которые проезжает каждая машина
            else if (chance == 3 || chance == 4)
            {
                new_solution.RegenerateCarPathLengths();
            }
            // третий тип: обе мутации
            else if (chance == 5)
            {
                int[] shops = new_solution.GetShops();
                int first = rand.Next(0, number_of_shops);
                int second;
                do
                {
                    second = rand.Next(0, number_of_shops);
                }
                while (first == second);
                int x = shops[first];
                shops[first] = shops[second];
                shops[second] = x;
                new_solution.RegenerateCarPathLengths();
            }
            return new_solution;
        }


        // Селекция
        // Отбираем каким-то образом решения
        private void Selection(out Solution best_solution, out int best_function_result)
        {
            List<Solution>[] groups = new List<Solution>[InitialNumber];
            bool[] is_assigned = new bool[population.Count];
            for (int i = 0; i < population.Count; ++i)
            {
                int position = Global.GetRandomFree(is_assigned, population.Count - i);
                if (i % SelectionGroupSize == 0)
                {
                    groups[i / SelectionGroupSize] = new List<Solution>();
                }
                groups[i / SelectionGroupSize].Add(population[position]);
                is_assigned[position] = true;
            }

            List<Solution> new_population = new List<Solution>();
            List<int> function_results = new List<int>();
            // отбираем наилучшее решение среди каждой группы
            foreach(List<Solution> group in groups)
            {
                int minimum_index = 0;
                int minimum_function_result = ObjectiveFunction(group[0]);
                for (int i = 1; i < group.Count; ++i)
                {
                    int function_result = ObjectiveFunction(group[i]);
                    if (minimum_function_result > function_result)
                    {
                        minimum_function_result = function_result;
                        minimum_index = i;
                    }
                }
                new_population.Add(group[minimum_index]);
                function_results.Add(minimum_function_result);
            }
            population = new_population;
            int minimum = function_results[0];
            int index = 0;
            for (int i = 1; i < population.Count; ++i)
            {
                if (function_results[i] < minimum)
                {
                    minimum = function_results[i];
                    index = i;
                }
            }
            best_solution = population[index].Copy();
            best_function_result = function_results[index];
        }
    }
}
