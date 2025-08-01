

import torch
import torch.nn as nn
from torchvision.datasets import MNIST
from torchvision import transforms
from torchinfo import summary
from torch.utils.data import Dataset, DataLoader
import torch.optim as optim
import os
import numpy as np

augmentations = transforms.Compose([transforms.ToTensor()])

train_dataset = MNIST(r"C:\Users\noahg\OneDrive\Documents\Coding Stuff\MNIST diffusion", download=True, train=True, transform=augmentations)
test_dataset = MNIST(r"C:\Users\noahg\OneDrive\Documents\Coding Stuff\MNIST diffusion", download=True, train=False, transform=augmentations)

device = torch.device("cuda")

class Model(nn.Module):
    def __init__(self):
        super(Model, self).__init__()
        self.layer1 = nn.Linear(784, 8)
        self.layer2 = nn.Linear(8, 10)

    def forward(self, input):
        input = input.view(input.size(0), -1)
        input = self.layer1(input)
        input = self.layer2(input)
        return input

model = Model()

# summary(model, input_size=(1, 1, 28, 28), device="cuda", dtypes=[torch.float16])

epochs = 30
batch_size = 16

model.train()

train_dataloader =  DataLoader(train_dataset, batch_size=batch_size, shuffle=True)
test_dataloader =  DataLoader(test_dataset, batch_size=batch_size, shuffle=True)

model = Model().to(device)
criterion = nn.CrossEntropyLoss()
optimizer = optim.Adam(model.parameters(), lr=1e-4)

for epoch in range(epochs):
    model.train()
    running_loss = 0.0
    correct = 0
    total = 0

    for images, labels in train_dataloader:
        images = images.to(device)
        labels = labels.to(device)

        outputs = model(images)
        loss = criterion(outputs, labels)

        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

        running_loss += loss.item() * images.size(0)

        _, predicted = torch.max(outputs, 1)
        correct += (predicted == labels).sum().item()
        total += labels.size(0)

    epoch_loss = running_loss / total
    epoch_accuracy = correct / total * 100
    print(f"Epoch [{epoch + 1}/{epochs}] - Loss: {epoch_loss:.4f}, Accuracy: {epoch_accuracy:.2f}%")

model.eval()
running_loss = 0.0
correct = 0
total = 0

for images, labels in test_dataloader:
    images = images.to(device)
    labels = labels.to(device)

    # Forward pass
    outputs = model(images)
    loss = criterion(outputs, labels)

    # Accumulate loss
    running_loss += loss.item() * images.size(0)

    # Compute accuracy
    _, predicted = torch.max(outputs, 1)
    correct += (predicted == labels).sum().item()
    total += labels.size(0)

# Epoch metrics
epoch_loss = running_loss / total
epoch_accuracy = correct / total * 100
print(f"Epoch [{epoch + 1}/{epochs}] - Loss: {epoch_loss:.4f}, Accuracy: {epoch_accuracy:.2f}%")

np.set_printoptions(threshold=np.inf)
with open("weights.txt", "w") as file:
    np.set_printoptions(precision=3, suppress=True)

    # Layer 1
    w1 = model.layer1.weight.data.cpu().numpy()
    b1 = model.layer1.bias.data.cpu().numpy()
    file.write(f"Weights Layer 1:\n{np.array2string(w1, separator=', ')}\n\n")
    file.write(f"Bias Layer 1:\n{np.array2string(b1, separator=', ')}\n\n")

    # Layer 2
    w2 = model.layer2.weight.data.cpu().numpy()
    b2 = model.layer2.bias.data.cpu().numpy()
    file.write(f"Weights Layer 2:\n{np.array2string(w2, separator=', ')}\n\n")
#     file.write(f"Bias Layer 2:\n{np.array2string(b2, separator=', ')}\n\n")
