using System;
using System.Collections.Generic;

namespace mtsp
{
    // Класс уже оперирует над измененным (полным) графом
    public class MtspSolver
    {
        public static int MaximumSameResults = 1000;
        public static int InitialNumber = 16;
        public static int CrossoverNumber = InitialNumber;  // т. е. + InitialNumber особей
        public static int MutationNumber = InitialNumber;  // т. е. + еще InitialNumber особей (итого 3 * InitialNumber)
        // следовательно для получения изначально числа, нужно оставить 2/3 особей
        public static int SelectionGroupSize =
                (InitialNumber + CrossoverNumber + MutationNumber) / InitialNumber;

        private readonly Graph graph;
        private readonly int number_of_cars;
        private readonly int number_of_shops;
        private List<Solution> population;
        private readonly List<int> global_shops;
        private readonly int[] storage_distance;
        private readonly Random rand;

        // конструктор
        // Мы преобразовываем глобальное решение в локальное (т. е. индексы из изначально графа
        // преобразуются в индексы в полного графа
        public MtspSolver(Graph graph, List<int> global_shops, int storage,
                int[] storage_distance, int number_of_cars)
        {
            rand = new Random();
            population = new List<Solution>();

            this.graph = graph;
            this.global_shops = global_shops;
            this.number_of_cars = number_of_cars;
            this.storage_distance = storage_distance;
            number_of_shops = graph.GetNumberOfShops();
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
                Mutation();
                Crossover();
                Selection(out best_solution, out function_result);

                // отсчитываем циклы с последнего изменения лучшего результата
                if (last != function_result) {
                    cycles += i;
                    last = function_result;
                    // Выводим новый лучши результат
                    Console.WriteLine(function_result);
                    i = 0;
                } else {
                    ++i;
                }
            }
            cycles += i;
            // Выводим итоговый лучший результат и последовательность магазинов
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
                    distance += graph.GetWeight(previous_shop, car_path[i]);
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
        private void Crossover()
        {
            if (number_of_shops < 2) {
                return;
            }

            for (int number = 0; number < CrossoverNumber; ++number)
            {
                // Выбираются две случайные хромосомы
                int first_chromosome = rand.Next(0, population.Count);
                int second_chromosome;
                do {
                    second_chromosome = rand.Next(0, population.Count);
                } while (first_chromosome == second_chromosome);
                Solution first = population[first_chromosome];
                Solution second = population[second_chromosome];

                // Скрещивание
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
                population.Add(child);
            }
        }

        // Мутация 
        // Изменяем каким-то образом решения из population
        private void Mutation()
        {
            // Выполняем MutationNumber раз
            for (int number = 0; number < MutationNumber; ++number) {
                int chromosome = rand.Next(0, population.Count);

                Solution new_solution = population[chromosome].Copy();
                // Первый тип: меняем местами случайные элементы среди магазинов
                int chance = rand.Next(0, 2);
                
                if (chance == 1) {//0 || chance == 1 || chance == 2) {  // Меняем местами
                    MutateSwap(new_solution);
                } else {// if (chance == 3 || chance == 4) {  // вычитание и прибавление длины пути
                    MutateAdjust(new_solution);
                } /*else if (chance == 5) {      // обе мутации
                    MutateSwap(new_solution);
                    MutateAdjust(new_solution);
                }*/
                population.Add(new_solution);
            }
        }

        private void MutateSwap(Solution solution)
        {
            int[] shops = solution.GetShops();
            int first_shop = rand.Next(0, number_of_shops);
            int second_shop = rand.Next(0, number_of_shops);
            while (first_shop == second_shop) {
                second_shop = rand.Next(0, number_of_shops);
            }
            int x = shops[first_shop];
            shops[first_shop] = shops[second_shop];
            shops[second_shop] = x;
        }

        private void MutateAdjust(Solution solution)
        {
            int[] cars = solution.GetCarPathLengths();
            int first_car = rand.Next(0, number_of_cars);
            int second_car = rand.Next(0, number_of_cars);
            while (first_car == second_car) {
                second_car = rand.Next(0, number_of_cars);
            }

            if (cars[first_car] == 1) {
                return;
            }
            int adjustment = rand.Next(1, cars[first_car] - 1);
            cars[first_car] -= adjustment;
            cars[second_car] += adjustment;
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
