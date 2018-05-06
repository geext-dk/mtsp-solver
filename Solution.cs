using System.Collections.Generic;
using System.Collections;

namespace mtsp
{

    // Класс решений. Содержит конкретное решение
    // Доступ к пути какой-то машины: метод GetCarPath
    public class Solution : IEnumerable<List<int>>
    {
        private List<int>[] solution;

        // Конструктор. Не уверен пока, какие именно конструкторы нужны
        // Пусть будет пока такой - составляет класс из массива списков
        public Solution(List<int>[] data)
        {
            this.solution = data;
        }

        List<int> GetCarPath(int car_number)
        {
            if (car_number >= 0 && car_number < solution.Length)
            {
                return solution[car_number];
            }
            else
            {
                return null;
            }
        }

        // поддержка foreach
        public IEnumerator<List<int>> GetEnumerator()
        {
            foreach (List<int> car_path in solution)
            {
                yield return car_path;
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
}