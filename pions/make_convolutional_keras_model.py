### Define CNN Model ###

from keras.models import Sequential
from keras.optimizers import Adam, SGD
from keras.initializers import TruncatedNormal
#from keras import initializations
from keras.layers import Input, Dense, Dropout, Flatten, Conv2D, MaxPooling2D, Reshape
#from keras.callbacks import ReduceLROnPlateau

# Define initialization
#def normal(shape, name=None):
#    return initializations.normal(shape, scale=0.05, name=name)

model = Sequential()
model.add(Reshape((32, 32, 1), input_shape=(1024,)))
model.add(Conv2D(12, kernel_size=(2,2), kernel_initializer='TruncatedNormal', activation='relu', padding='same', input_shape=(32,32,1) ) )
model.add(Conv2D(12, kernel_size=(2,2), kernel_initializer='TruncatedNormal', activation='relu', padding='same', input_shape=(32,32,1) ) )
#model.add(Convolution2D(10, 3, 3 , kernel_initializer='TruncatedNormal', activation='relu', border_mode='same'))
#model.add(Conv2D(10, activation='relu', kernel_size=(3,3), padding='same'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Conv2D(12, kernel_size=(2,2), kernel_initializer='TruncatedNormal', activation='relu', padding='same', input_shape=(32,32,1) ) )
model.add(Conv2D(12, kernel_size=(2,2), kernel_initializer='TruncatedNormal', activation='relu', padding='same', input_shape=(32,32,1) ) )
#model.add(Convolution2D(10, 3, 3 , kernel_initializer='TruncatedNormal', activation='relu', border_mode='same'))
#model.add(Conv2D(10, activation='relu', kernel_size=(3,3), padding='same'))
model.add(MaxPooling2D(pool_size=(2, 2)))
#model.add(Conv2D(32, activation='relu', kernel_size=3, padding='same', kernel_initializer='TruncatedNormal'))
#model.add(Conv2D(32, activation='relu', kernel_size=3, padding='same', kernel_initializer='TruncatedNormal'))
#model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Flatten())
model.add(Dense(64, activation='relu'))
model.add(Dropout(0.2))
model.add(Dense(32, activation='relu'))
model.add(Dropout(0.2))
model.add(Dense(2, activation='sigmoid'))
model.compile(loss='binary_crossentropy', optimizer=Adam(lr=0.001), metrics=['accuracy'])
#model.compile(loss='binary_crossentropy', optimizer=SGD(lr=0.01), metrics=['accuracy',])
model.save('model_cnn.h5')
model.summary()
