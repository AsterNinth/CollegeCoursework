import java.util.Arrays;
import java.util.Scanner;

public class RodCollecting {
    private static int scale = 1000;

    public static void main(String[] args) {
        // --------------------
        // Variable declaration
        // --------------------

        Scanner sc;
        int capacity, number_of_rods, total_val, temp;
        Rod[] rods;
        int[] answer;

        // -------------------
        // Variable assignment
        // -------------------

        sc = new Scanner(System.in);
        capacity = sc.nextInt()*scale;
        number_of_rods = sc.nextInt();

        rods = new Rod[number_of_rods];
        total_val = 0;

        // Read in the rods but also keep track of the combined value of all rods
        for (int i = 0; i < number_of_rods; i++) {
            rods[i] = new Rod(sc.nextInt(), sc.nextFloat());
            total_val += rods[i].value;
        }

        // Create an answer array that is indexed by the possible
        // values from the given rods and fill it with a value not
        // reachable by the combined weight of the rods. Assign index 0
        // to 0 in order to have a starting point for the first update loop
        answer = new int[total_val+1];
        Arrays.fill(answer, capacity+1);
        answer[0] = 0;

        sc.close();

        // -------------------
        // Dynamic Programming
        // -------------------

        // For each rod, we update the loop in reverse order to find
        // the minimum weight necessary for each value being indexed
        for (Rod rod : rods) {
            for (int ind = answer.length - 1; ind >= rod.value; ind--) {
                temp = rod.weight + answer[ind - rod.value];

                if (answer[ind] > temp)
                    answer[ind] = temp;
            }
        }

        // -------------
        // Print Results
        // -------------

        // Print the index of the last cell that has
        // a weight below or equal to the capacity
        for (int i = answer.length - 1; i >= 0; i--) {
            if (answer[i] <= capacity) {
                System.out.println(i);
                break;
            }
        }
    }

    // =================
    // DATA STRUCTURE(S)
    // =================

    private static class Rod {
        int value;
        int weight;

        public Rod(int value, float weight) {
            this.value = value;
            this.weight = (int) (weight*scale);
        }
    }
}
