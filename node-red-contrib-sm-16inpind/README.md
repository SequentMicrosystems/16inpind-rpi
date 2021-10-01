# node-red-contrib-sm-16inpind

This is the node-red node to control Sequent Microsystems [16-INPUTS Stackable Card for Raspberry Pi](https://sequentmicrosystems.com/collections/all-io-cards/products/16-universal-inputs-3-48vac-dc-8-layer-stackable-card-br-for-raspberry-pi).

## Manual Install

Clone or update the repository, follow the instructions fron the [first page.](https://github.com/SequentMicrosystems/16inpind-rpi)

In your node-red user directory, tipicaly ~/.node-red,

```bash
~$ cd ~/.node-red
```

Run the following command:

```bash
~/.node-red$ npm install ~/16inpind-rpi/node-red-contrib-sm-16inpind
```

In order to see the node in the palette and use-it you need to restart node-red. If you run node-red as a service:
 ```bash
 ~$ node-red-stop
 ~$ node-red-start
 ```

## Usage

After install and restart the node-red you will see on the node palete, under Sequent Microsystems category the "16inpind" node.This node will return the state of one of 16 inputs. 
The card stack level and channel number can be set in the dialog screen or dinamicaly thru ``` msg.stack``` and ``` msg.channel ```.

## Important note

This node is using the I2C-bus package from @fivdi, you can visit his work on github [here](https://github.com/fivdi/i2c-bus). 
The inspiration for this node came from @nielsnl68 work with [node-red-contrib-i2c](https://github.com/nielsnl68/node-red-contrib-i2c).Thank both for the great job.
