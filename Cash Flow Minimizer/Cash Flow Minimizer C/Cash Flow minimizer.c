#include <stdio.h>
#include <stdlib.h>

#define MAX_PEOPLE 100


typedef struct {
    int data[MAX_PEOPLE];
    int front;
    int rear;
} Queue;


void initQueue(Queue *q) {
    q->front = 0;
    q->rear = 0;
}

void enqueue(Queue *q, int value) {
    if (q->rear < MAX_PEOPLE) {
        q->data[q->rear++] = value;
    } else {
        printf("Queue is full.\n");
    }
}

int dequeue(Queue *q) {
    if (q->front < q->rear) {
        return q->data[q->front++];
    } else {
        return -1;
    }
}

typedef struct {
    int from;
    int to;
    int amount;
} Transaction;

void pushTransaction(Transaction stack[], int *top, int from, int to, int amount) {
    if (*top < MAX_PEOPLE) {
        stack[*top].from = from;
        stack[*top].to = to;
        stack[*top].amount = amount;
        (*top)++;
    } else {
        printf("Stack is full.\n");
    }
}

Transaction popTransaction(Transaction stack[], int *top) {
    if (*top > 0) {
        (*top)--;
        return stack[*top];
    } else {
        Transaction empty = { -1, -1, -1 };
        return empty;
    }
}

void minimizeCashFlowRec(int net[], int n, Queue *oweQueue, Queue *owedQueue, Transaction stack[], int *top) {
    int maxCredit = -1, maxDebit = -1;

    for (int i = 0; i < n; i++) {
        if (net[i] > 0 && (maxCredit == -1 || net[i] > net[maxCredit])) {
            maxCredit = i;
        }
        if (net[i] < 0 && (maxDebit == -1 || net[i] < net[maxDebit])) {
            maxDebit = i;
        }
    }

    if (maxCredit == -1 || maxDebit == -1 || net[maxCredit] == 0 || net[maxDebit] == 0)
        return;

    int minAmount = (net[maxCredit] < -net[maxDebit]) ? net[maxCredit] : -net[maxDebit];
    net[maxCredit] -= minAmount;
    net[maxDebit] += minAmount;

    printf("Person %d pays %d to Person %d\n", maxDebit, minAmount, maxCredit);

    pushTransaction(stack, top, maxDebit, maxCredit, minAmount);

    minimizeCashFlowRec(net, n, oweQueue, owedQueue, stack, top);
}

void minimizeCashFlow(int transactions[MAX_PEOPLE][MAX_PEOPLE], int n, Transaction stack[], int *top) {
    int net[MAX_PEOPLE] = {0};

    for (int p = 0; p < n; p++) {
        for (int q = 0; q < n; q++) {
            net[p] += transactions[q][p] - transactions[p][q];
        }
    }

    printf("\n=====================================\n");
    printf("       Net Balance Summary\n");
    printf("=====================================\n");
    for (int i = 0; i < n; i++) {
        if (net[i] > 0)
            printf("Person %d is owed %d\n", i, net[i]);
        else if (net[i] < 0)
            printf("Person %d owes %d\n", i, -net[i]);
        else
            printf("Person %d has no balance\n", i);
    }

    Queue oweQueue, owedQueue;
    initQueue(&oweQueue);
    initQueue(&owedQueue);

    for (int i = 0; i < n; i++) {
        if (net[i] < 0) {
            enqueue(&oweQueue, i);
        } else if (net[i] > 0) {
            enqueue(&owedQueue, i);
        }
    }

    printf("\n=====================================\n");
    printf("       Minimized Cash Flow\n");
    printf("=====================================\n");
    minimizeCashFlowRec(net, n, &oweQueue, &owedQueue, stack, top);
}

void undoLastTransaction(Transaction stack[], int *top) {
    if (*top > 0) {
        Transaction lastTransaction = popTransaction(stack, top);
        printf("\n=====================================\n");
        printf("         Undo Last Transaction\n");
        printf("=====================================\n");
        printf("Undo: Person %d was to pay %d to Person %d\n", lastTransaction.from, lastTransaction.amount, lastTransaction.to);
    } else {
        printf("No transactions to undo.\n");
    }
}

int main() {
    int n;
    printf("Enter the number of people involved: ");
    scanf("%d", &n);

    int transactions[MAX_PEOPLE][MAX_PEOPLE] = {0};

    printf("Enter transactions in the format (from to amount), or -1 to finish:\n");
    int from, to, amount;
    while (1) {
        printf("From: ");
        scanf("%d", &from);
        if (from == -1) break;
        printf("To: ");
        scanf("%d", &to);
        printf("Amount: ");
        scanf("%d", &amount);
        transactions[from][to] += amount;
    }

    Transaction stack[MAX_PEOPLE];
    int top = 0;

    minimizeCashFlow(transactions, n, stack, &top);

    char choice;
    printf("\nDo you want to undo the last transaction? (y/n): ");
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y') {
        undoLastTransaction(stack, &top);
    }

    printf("\n=====================================\n");
    printf("        Transaction Summary\n");
    printf("=====================================\n");
    printf("Thank you for using the Cash Flow Minimizer!\n");

    return 0;
}
