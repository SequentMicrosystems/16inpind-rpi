# node-red-contrib-sm-16inpind

This is the node-red node to control Sequent Microsystems [16-INPUTS Stackable Card for Raspberry Pi](https://sequentmicrosystems.com/collections/all-io-cards/products/16-universal-inputs-3-48vac-dc-8-layer-stackable-card-br-for-raspberry-pi).

## Manual Install

Clone or update the repository, follow the instructions from the [first page.](https://github.com/SequentMicrosystems/16inpind-rpi)

In your node-red user directory, typically ~/.node-red,

```bash
~$ cd ~/.node-red
```

Run the following command:

```bash
~/.node-red$ npm install ~/16inpind-rpi/node-red-contrib-sm-16inpind
```

To see the node in the palette and use it you need to restart node-red. If you run node-red as a service:
 ```bash
 ~$ node-red-stop
 ~$ node-red-start
 ```

## Usage

After installing and restarting the node-red you will see on the node palette, under the Sequent Microsystems category the "16inpind" node.
This node will output the state of one of 16 inputs if the ```channel``` parameter is between 1 and 16 including. The node will output a bitmap of all 16 inputs if the ```channel``` parameter is 0.
The card stack level and channel number can be set in the dialog screen or dynamically thru ``` msg.stack``` and ``` msg.channel ```.
## Important note

This node is using the I2C-bus package from @fivdi, you can visit his work on GitHub [here](https://github.com/fivdi/i2c-bus). 
The inspiration for this node came from @nielsnl68 work with [node-red-contrib-i2c](https://github.com/nielsnl68/node-red-contrib-i2c).Thank you both for a great job.
