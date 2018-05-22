using System;
using System.Collections.Generic;

namespace mtsp
{
    public class MtspSolver
    {
        // ========================
        public static int MaximumSameResults = 1000;
        public static int InitialNumber = 16;
        public static int CrossoverNumber = InitialNumber;
        public static int MutationNumber = InitialNumber;
        public static int SelectionGroupSize =
            (InitialNumber + CrossoverNumber + MutationNumber) / InitialNumber;
        // ========================

        private readonly Graph graph;
        private readonly int number_of_cars;
        private readonly int number_of_shops;
        private List<Solution> population;
        private readonly int[] global_shops;
        private readonly int[] distance_from_storage_to;
        private readonly Random rand;

        public MtspSolver(Graph graph, int[] global_shops, int storage,
                int[] distance_from_storage_to, int number_of_cars)
        {
            rand = new Random();
            population = new List<Solution>();

            this.graph = graph;
            this.global_shops = global_shops;
            this.number_of_cars = number_of_cars;
            this.distance_from_storage_to = distance_from_storage_to;
            number_of_shops = graph.GetNumberOfShops();
        }

        public void Solve()
        {
            int cycles = 0;

            GenerateInitialPopulation();

            int i = 0;
            int best_result = 0;
            int last_best_result = 0;
            Solution best_solution;
            do {
                Mutation();
                Crossover();
                Selection(out best_solution, out best_result);

                if (last_best_result != best_result) {
                    cycles += i;
                    i = 0;
                    last_best_result = best_result;
                    Console.WriteLine(best_result);
                } else {
                    ++i;
                }
            } while (i < MaximumSameResults);
            cycles += i;
            
            Console.WriteLine(best_result + ": " + best_solution.GetGlobalString(global_shops));
            Console.WriteLine("Cycles: " + cycles);
        }

        private int ObjectiveFunction(Solution solution)
        {
            int maximum_distance = 0;
            foreach (int[] car_path in solution) {
                int distance = distance_from_storage_to[car_path[0]];
                int previous_shop = car_path[0];
                for (int i = 1; i < car_path.Length; ++i) {
                    distance += graph.GetWeight(previous_shop, car_path[i]);
                    previous_shop = car_path[i];
                }
                distance += distance_from_storage_to[previous_shop];
                if (distance > maximum_distance) {
                    maximum_distance = distance;
                }
            }
            return maximum_distance;
        }

        private void GenerateInitialPopulation()
        {
            for (int i = 0; i < InitialNumber; ++i) {
                this.population.Add(new Solution(number_of_shops, number_of_cars));
            }
        }

        private void Crossover()
        {
            if (number_of_shops < 2) {
                return;
            }

            for (int number = 0; number < CrossoverNumber; ++number) {
                int first_chromosome = rand.Next(0, population.Count);
                int second_chromosome;
                do {
                    second_chromosome = rand.Next(0, population.Count);
                } while (first_chromosome == second_chromosome);
                Solution first = population[first_chromosome];
                Solution second = population[second_chromosome];


                int[] second_shops_array = second.GetShopsArray();
                Solution child = first.Copy();
                int[] child_shops_array = child.GetShopsArray();
                bool[] is_position_assigned = new bool[number_of_shops];
                bool[] is_shop_assigned = new bool[number_of_shops];


                int length = rand.Next(1, number_of_shops - 1);
                int start_with = rand.Next(1, number_of_shops - length);
                for (int i = start_with; i < length + start_with; ++i) {
                    is_position_assigned[i] = true;
                    is_shop_assigned[child_shops_array[i]] = true;
                }

                for (int i = 0; i < number_of_shops; ++i) {
                    if (is_position_assigned[i]) {
                        continue;
                    }
                    
                    foreach (int second_shop in second_shops_array) {
                        if (!is_shop_assigned[second_shop]) {
                            child_shops_array[i] = second_shop;
                            is_position_assigned[i] = true;
                            is_shop_assigned[second_shop] = true;
                            break;
                        }
                    }
                }
                population.Add(child);
            }
        }

        private void Mutation()
        {
            for (int number = 0; number < MutationNumber; ++number) {
                int chromosome = rand.Next(0, population.Count);

                Solution new_solution = population[chromosome].Copy();
                
                int chance = rand.Next(0, 2);
                
                if (chance == 1) {
                    MutateSwap(new_solution);
                } else {
                    MutateAdjust(new_solution);
                }
                population.Add(new_solution);
            }
        }

        private void MutateSwap(Solution solution)
        {
            int[] shops = solution.GetShopsArray();
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
            int[] cars = solution.GetPathLengths();
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

        private void Selection(out Solution best_solution, out int best_function_result)
        {
            List<Solution>[] groups = new List<Solution>[InitialNumber];
            bool[] is_assigned = new bool[population.Count];
            for (int i = 0; i < population.Count; ++i) {
                int position = Global.GetRandomFree(is_assigned, population.Count - i);
                if (i % SelectionGroupSize == 0) {
                    groups[i / SelectionGroupSize] = new List<Solution>();
                }
                groups[i / SelectionGroupSize].Add(population[position]);
                is_assigned[position] = true;
            }

            List<Solution> new_population = new List<Solution>();
            List<int> function_results = new List<int>();
            
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
            for (int i = 1; i < population.Count; ++i) {
                if (function_results[i] < minimum) {
                    minimum = function_results[i];
                    index = i;
                }
            }
            best_solution = population[index].Copy();
            best_function_result = function_results[index];
        }
    }
}
