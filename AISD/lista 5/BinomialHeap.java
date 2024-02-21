import java.util.*;

public class BinomialHeap {
    public int operationComparisons = 0;

    private static class BinomialTree {
        int key;
        List<BinomialTree> children;
        int order;

        BinomialTree(int key) {
            this.key = key;
            this.children = new ArrayList<>();
            this.order = 0;
        }

        void addAtEnd(BinomialTree t) {
            children.add(t);
            order++;
        }
    }

    public List<BinomialTree> trees;

    public BinomialHeap() {
        this.trees = new ArrayList<>();
    }

    public int extractMin() {
        if (trees.isEmpty()) {
            System.out.println("Heap is empty");
            System.exit(0);
        }

        BinomialTree smallestNode = trees.get(0);
        for (BinomialTree tree : trees) {
            operationComparisons++;
            if (tree.key < smallestNode.key) {
                smallestNode = tree;
            }
        }

        trees.remove(smallestNode);

        BinomialHeap h = new BinomialHeap();
        h.trees = smallestNode.children;
        merge(h);

        System.out.println("delete " + smallestNode.key);
        return smallestNode.key;
    }

    private void combineRoots(BinomialHeap h) {
        trees.addAll(h.trees);
        trees.sort(Comparator.comparingInt(tree -> tree.order));
    }

    public void merge(BinomialHeap h) {
        combineRoots(h);
        if (trees.isEmpty()) {
            return;
        }

        int i = 0;
        while (i < trees.size() - 1) {
            BinomialTree current = trees.get(i);
            BinomialTree after = trees.get(i + 1);
            if (current.order == after.order) {
                if (i + 1 < trees.size() - 1 && trees.get(i + 2).order == after.order) {
                    BinomialTree afterAfter = trees.get(i + 2);
                    operationComparisons++;
                    if (after.key < afterAfter.key) {
                        after.addAtEnd(afterAfter);
                        trees.remove(i + 2);
                    } else {
                        afterAfter.addAtEnd(after);
                        trees.remove(i + 1);
                    }
                } else {
                    operationComparisons++;
                    if (current.key < after.key) {
                        current.addAtEnd(after);
                        trees.remove(i + 1);
                    } else {
                        after.addAtEnd(current);
                        trees.remove(i);
                    }
                }
            }
            i++;
        }
    }

    public void insert(int key) {
        System.out.println("instert: " + key);
        BinomialHeap g = new BinomialHeap();
        g.trees.add(new BinomialTree(key));
        merge(g);
    }

    public static void main(String[] args) {
        BinomialHeap binomialHeap = new BinomialHeap();
        BinomialHeap binomialHeap2 = new BinomialHeap();
        Random ran = new Random();
        int nxt;
        System.out.println("H1");
        for(int i = 0; i<10;i++) {
            nxt = ran.nextInt(100000);
            binomialHeap.insert(nxt);
        }
        System.out.println("H2");
        for(int i = 0; i<10;i++) {
            nxt = ran.nextInt(100000);
            binomialHeap2.insert(nxt);
        }
        System.out.println("Merge");
        binomialHeap.merge(binomialHeap2);
        System.out.println("extracting");
        while (true) {
            binomialHeap.extractMin();
        }
    }
}
