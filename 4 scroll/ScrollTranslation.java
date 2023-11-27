import java.util.Arrays;
import java.util.Scanner;

public class ScrollTranslation {
    public static void main(String[] args) {
        // --------------------
        // Variable declaration
        // --------------------

        Scanner sc;
        int a, b, n;
        Scroll[] scrolls;

        // -------------------
        // Variable assignment
        // -------------------

        sc = new Scanner(System.in); // Reads in from standard input
        n = sc.nextInt(); // the number of scrolls to read in
        scrolls = new Scroll[n]; // array to hold all the scrolls

        // Reads in and stores the scrolls
        for (int i = 0; i < n; i++) {
            a = sc.nextInt();
            b = sc.nextInt();

            scrolls[i] = new Scroll(a, b);
        }
        // Finished reading from standard in
        sc.close();

        // -------------------
        // Variable assignment
        // -------------------

        // Sorts the scrolls based on greedy observations (check the compareTo method in the Scroll class)
        Arrays.sort(scrolls);

        // Perform the sweep and prints the result
        System.out.println(sweep(scrolls));

    }

    // ================
    // HELPER FUNCTIONS
    // ================

    // Input(s):
    //     scrolls: a Scroll array
    // Description:
    //     returns how long is takes for both translators to
    //     translate the scrolls based on how they are currently arranged
    private static long sweep(Scroll[] scrolls) {
        // time it takes translator a to translate
        // never exceeds 1,000,000,000 so it can be an int
        int translate_a;
        // time it takes translator b to translate
        // may exceed 1,000,000,000 so it has to be a long
        long translate_b = 0;
        // total time it takes both translators to translate
        // may exceed 1,000,000,000 so it has to be a long
        long total_time = 0;

        for (Scroll scroll : scrolls) {
            // Record the time for translator a to translate the scroll
            translate_a = scroll.a;
            // Subtract how long a took from translator b's remaining time unless b has no scrolls left
            translate_b = Math.max(0, translate_b - translate_a);
            // add the time passed to the total time
            total_time += translate_a;

            // Since a just finished a translation, b has to translate that
            translate_b += scroll.b;
        }

        // a has no more to translate, so we just wait for b to finish
        total_time += translate_b;

        return total_time;
    }

    // =================
    // DATA STRUCTURE(S)
    // =================

    private static class Scroll implements Comparable<Scroll>{
        int a, b, rank;

        public Scroll(int a, int b) {
            this.a = a;
            this.b = b;

            // Going off the observation that we should do documents with faster
            // translator a times than b times so that we have less time where b isn't
            // doing anything, we give a scroll a rank based on their a time vs b time.
            if (a < b) { // takes less time for translator a than b
                this.rank = 1;
            }
            else if (a == b) { // takes the same amount of time for translators a and b
                this.rank = 2;
            }
            else { // takes more time for translator a than b
                this.rank = 3;
            }
        }

        @Override
        public int compareTo(Scroll o) {
            // Since rank is based in a vs b time, we can sort based on that if two scrolls have different ranks
            // Sorts the general order in rank 1, rank 2, rank 3 order
            if (this.rank != o.rank) {
                return Integer.compare(this.rank, o.rank);
            }
            // If both scrolls are the same rank, we compare them based on what rank they are in
            else {
                // We want to order them so that we build up as much of a backlog as we can for translator b
                // Therefore, we put the ones with shorter translator a time in front
                if (this.rank == 1) {
                    return Integer.compare(this.a, o.a);
                }
                // Since we are using up as much of the backlog as we are
                // adding, it does not matter what order we do these scrolls
                else if (this.rank == 2) {
                    return 0;
                }
                // Since we are now using up more of the backlog than we are
                // adding, we want to focus on adding as much b time as we can
                else {
                    return Integer.compare(o.b, this.b);
                }
            }
        }
    }
}
