using System;

namespace mtsp
{
    public class Global
    {
        public static int GetRandomFree(bool[] is_assigned, int number_of_free)
        {
            Random rand = new Random();
            int random_position = rand.Next(0, number_of_free - 1);
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
