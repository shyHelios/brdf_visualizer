import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

steps = 40

uArr = np.arange(0, 1, 1/steps).tolist()
vArr = np.arange(0, 1, 1/steps).tolist()

normalVector = [0, 0, 1]

M_2PI = (2.0 * np.pi)


def inversesqrt(num):
    return 1 / np.sqrt(num)


def normalize(vec):
    inv = inversesqrt(dot(vec, vec))

    return [vec[0] * inv, vec[1] * inv, vec[2] * inv]


def dot(vec1, vec2):
    return vec1[0] * vec2[0] + vec1[1] * vec2[1] + vec1[2] * vec2[2]


def hemisphereSampling(randomU: float, randomV: float, normal):

    u = np.cos(M_2PI * randomU) * np.sqrt(randomV * (1.0 - randomV))
    v = np.sin(M_2PI * randomU) * np.sqrt(randomV * (1.0 - randomV))
    w = 1.0 - randomV

    normalized = normalize([u, v, w])

    if (dot(normalize(normal), normalized) < 0):
        normalized[0] *= -1.0
        normalized[1] *= -1.0
        normalized[2] *= -1.0

    # return [normalized[0], normalized[1], normalized[2]]
    return [0, 0, 0, normalized[0], normalized[1], normalized[2]]


if __name__ == "__main__":

    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')

    samples = [[], [], [], [], [], []]
    for u in uArr:
        for v in vArr:
            result = hemisphereSampling(u, v, normalVector)
            print(result)
            # plt.plot([0, 0, 0], result)
            for i in range(len(result)):
                samples[i].append(result[i])

    X = samples[0]
    Y = samples[1]
    Z = samples[2]
    U = samples[3]
    V = samples[4]
    W = samples[5]

    ax.quiver(X, Y, Z, U, V, W)
    ax.set_xlim([-1, 1])
    ax.set_ylim([-1, 1])
    ax.set_zlim([0, 2])
    plt.show()
