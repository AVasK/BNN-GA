import random
from copy import copy

def mean(vec):
    return sum(vec)/len(vec)

def crossover(chromo1, chromo2):
    assert len(chromo1) == len(chromo2)
    idx = random.randint(0, len(chromo1))
    return chromo1[:idx] + chromo2[idx:]

def mutate(chromo):
    chromo1 = copy(chromo)
    idx = random.randint(0, len(chromo)-1)
    chromo1[idx] = int(not(chromo[idx]))
    return chromo1

def stochastic_mutate(chromo, prob=0.05):
    if random.random() > prob:
        return mutate(chromo)
    return chromo

def n_point_mutate(chromo, n):
    c = copy(chromo)
    for _ in range(n):
        c = mutate(c)
    return c

def tour_select(population, loss_f, loss_prob=0.2):
    e1 = random.choice(population)
    e2 = random.choice(population)
    if loss_f(e1) > loss_f(e2):
        if random.random() > loss_prob:
            return e2
        else:
            return e1
    else:
        if random.random() > loss_prob:
            return e1
        else:
            return e2


def selection1(population, pop_size, loss_function):
    return [tour_select(population, loss_function) for _ in range(pop_size)]

def cross(population, pop_size):
    return [crossover(random.choice(population), random.choice(population)) for _ in range(pop_size)]


def argmin(array, criterion):
    if not array:
        return None
    arg = array[0]
    min = criterion(array[0])
    for elem in array:
        if criterion(elem) < min:
            min = criterion(elem)
            arg = elem
    return arg, min

def runGA(start_population : '[Chromosome]', population_size, iterations, loss_function, selection_strategy=selection1, stop_loss=0):
    population = start_population
    for iter in range(iterations):
        loss = argmin(population, loss_function)[1]
        if iter % 10 == 1:
            print('loss:', loss)
        if loss == 0:
            break
        mutated = list(map(stochastic_mutate, population))
        crossed = cross(mutated, population_size)
        selected = selection_strategy(crossed+population, 2*population_size, loss_function)
        population = selected


    return argmin(population, loss_function)


def f(x):
    return (x-2)*(x-5)

def convert(chromo):
    return sum([c*(2**i) for i,c in enumerate(chromo[::-1])])

def F(chromo):
    return f(convert(chromo))

def loss(chromo):
    return F(chromo)

def gen_population(clen, pop_size):
    chromos = []
    for _ in range(pop_size):
        genchromo = [int(random.random() > 0.5) for _ in range(clen)]
        chromos.append(genchromo)
    return chromos


def simple_bnn(chromo, i1, i2):
    w = chromo[:-3]
    out_w = chromo[-3:]
    w1 = w[:len(w)//2]
    w2 = w[len(w)//2:]

    #print([i1,i2], w, out_w)

    def op(i, w):
        if i == 0:
            i = -1
        if w == 0:
            w = -1
        return i * w

    def sign(a):
        return int(a>0)

    from math import exp
    def sigmoid(a):
        return 1 / (1+exp(-a))

    a1 = [op(i1,wi1) for wi1 in w1]
    a2 = [op(i2,wi2) for wi2 in w2]
    #print(a1)
    #print(a2)
    a = [ai1 + ai2 for ai1, ai2 in zip(a1, a2)]
    #print(a)
    as_bin = [sign(_a) for _a in a]
    #print(as_bin)

    # or sum([(sign(op...
    res = (lambda x : int(x > 0.5))(sigmoid(sum([(op(a,w)) for a,w in zip(as_bin, out_w)])))
    return res

def loss_on_input(chromo, i1, i2, target):
    output = simple_bnn(chromo, i1, i2)
    return output - target

def binary_inputs(*, dims=2):
    toBin = lambda i : '0'*(dims - len(str(bin(i))[2:])) + str(bin(i))[2:]
    return [[int(s) for s in toBin(i)] for i in range(2**dims)]

def loss_on_batch(chromo):
    batch = [
        [0, 0, 0],
        [0, 1, 1],
        [1, 0, 1],
        [1, 1, 0],
    ]

    MSE = 0.0
    for i1, i2, target in batch:
        loss = loss_on_input(chromo, i1, i2, target)
        MSE += loss * loss

    MSE /= len(batch)
    return MSE


if __name__ == "__main__":
    print(crossover([1,1,1,1], [0,0,0,0]))
    print(mutate([1,1,1,1]))

    print(convert([1,0,1,0]))

    #pop_size = 15
    #population = gen_population(5, pop_size)
    #solns = runGA(population, pop_size, 20, loss)
    #print(list(map(convert, solns)))

    #print(simple_bnn([1,0,0,0,0,0,1,1,1], 1, 0))
    #print(loss_on_batch([1,0,0,0,0,0,1,1,1]))

    pop_size = 25
    population = gen_population(9, pop_size)
    soln, mse  = runGA(population, pop_size, 100, loss_on_batch)
    print(simple_bnn(soln, 0,0))
    print(simple_bnn(soln, 0,1))
    print(simple_bnn(soln, 1,0))
    print(simple_bnn(soln, 1,1))
    #for soln in solns:
    #    print(simple_bnn(soln, 1,1))
