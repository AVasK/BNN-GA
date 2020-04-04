import random

def sign_activation(arr):
    def sign(x):
        return int(x>=0)
    return [sign(e) for e in arr]

from math import exp
def sigmoid(a):
    return 1 / (1+exp(-a))

def sigmoid_activation(arr):
    return [sigmoid(a) for a in arr]

def threshold_activation(arr):
    return [int(a>0.5) for a in arr]


class BNN:
    def __init__(self, layers : '[size]'):
        self.layers = layers
        self.weights = [ [] for _ in self.layers[:-1] ]
        for iw in range(len(self.weights)):
            layer_weights = []
            for index in range(self.layers[iw]):
                layer_weights.append([int(random.random() > 0.5) for _ in range(self.layers[iw+1])])
            self.weights[iw] = layer_weights

    def num_weights(self):
        num = 0
        for layer_size, next_layer_size in zip(self.layers[:-1], self.layers[1:]):
            num += layer_size * next_layer_size
        return num

    def weights_to_chromosome(self):
        chromosome = []
        for layer_weights in self.weights:
            for weights in layer_weights:
                for weight in weights:
                    chromosome.append(weight)

        return chromosome


    def chromosome_to_weights(self, chromosome):
        idx = 0
        for i_layer in range(len(self.weights)):
            for i_weights in range(len(self.weights[i_layer])):
                for i in range(len(self.weights[i_layer][i_weights])):
                    self.weights[i_layer][i_weights][i] = chromosome[idx]
                    idx += 1

        return self


    def forward(self, input):
        assert self.layers[0] == len(input)

        def product(xs, weights):
            assert len(xs) == len(weights)

            def fromBinary(x):
                if x == 0:
                    return -1
                return x

            res = [0 for _ in range(len(weights[0]))]
            for i_w in range(len(weights[0])):
                for i_x in range(len(xs)):
                    res[i_w] += fromBinary(weights[i_x][i_w]) * fromBinary(xs[i_x])

            #print(f"{xs}\n * \n{weights}\n = \n{res}")
            return res


        for i_next in range(1, len(self.layers)):
            input = product(input, self.weights[i_next-1])

            if i_next != len(self.layers) - 1:
                # Inner layer
                input = sign_activation(input)
            else:
                # Output layer
                input = threshold_activation(sigmoid_activation(input))

            """
            if i_next != len(self.layers) - 1:
                # Inner layer
                input = sigmoid_activation(input)
            else:
                # Output layer
                input = threshold_activation(sigmoid_activation(input))
            """

        return input


    def loss_on_batch(self, batch_inputs, batch_targets, verbose=False):
        MSE = 0.0
        for inp, tgt in zip(batch_inputs, batch_targets):
            if verbose:
                print("L:",inp, self.forward(inp), tgt)
            loss = self.forward(inp)[0] - tgt
            MSE += loss * loss

        MSE /= len(batch_targets)
        return MSE


    def __str__(self):
        out  = ' x '.join(list(map(str, self.layers))) + "\nwith weights:\n"
        out += '\n'.join(list(map(str, self.weights)))
        return out



if __name__ == "__main__":

    from GA import runGA, gen_population, binary_inputs


    bnn = BNN([3,20,20,1])
    print(bnn)
    bnn.weights_to_chromosome()
    bnn.chromosome_to_weights(gen_population(bnn.num_weights(), 1)[0])
    #print(bnn)
    print("with {", bnn.num_weights(), "} weights")
    #bnn.forward([1,1])
    #print(bnn.loss_on_batch([[0,0], [0,1], [1,0], [1,1]], [0,1,1,0]))



    def bnn_loss(chromo):
        global bnn
        bnn.chromosome_to_weights(chromo)
        #print(bnn, '\n\n')
        outputs = [0,0,1,1,1,1,0,0]
        return bnn.loss_on_batch(binary_inputs(dims=3), outputs)


    pop_size = 25
    population = gen_population(bnn.num_weights(), pop_size)
    #print(binary_inputs(dims=3))
    soln, mse  = runGA(population, pop_size, 400, bnn_loss)
    outputs = [0,0,1,1,1,1,0,0]
    print('Loss: ', bnn.chromosome_to_weights(soln).loss_on_batch(binary_inputs(dims=3), outputs, verbose=True))
