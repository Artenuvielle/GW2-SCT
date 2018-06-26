# Guild Wars 2 - Scrolling Combat Text (GW2SCT)

This is an addon for arcdps by deltaconnected (https://www.deltaconnected.com/arcdps/) and adds a highly customizable floating and scrolling combat text beside the users character. Incoming damage is displayed on the left side, outgoing damage is displayed on the right side.

### Prerequisites

GW2SCT requires ArcDPS to work. You can install arcdps by following the instructions in this site:
https://www.deltaconnected.com/arcdps/

### Installing

Go to the the [release page](https://github.com/Artenuvielle/GW2-SCT/releases) and download the newest release. Copy the file "d3d9_arcdps_sct.dll" into the "bin64/"" subfolder of your game directory.

If you want to add your own fonts copy them in the "addons/sct/fonts/" subfolder of your game directory. You can configure the output in the arcdps options panel (opened by default with Alt+Shift+T).

## Contributing

Feel free to contact me on [Reddit](https://www.reddit.com/user/Artenuvielle/) if you have suggestions or encounter any bugs not listed below.

## Known Bugs

* The addon seems to not be loaded correctly or at all on some machines. If you have any information why this may be happening or how to resolve this, please reach out to me on [Reddit](https://www.reddit.com/user/Artenuvielle/)
* Damage done to your pets/clones will be displayed as damage your pets do. Currently there is no way to differentiate between your pets and enemies when either of them deals damage the other.

## Authors

* **René Martin** - *Initial work* - [Artenuvielle](https://github.com/Artenuvielle)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

The source code includes a copy of the [imgui library](https://github.com/ocornut/imgui) which is also licensed under the MIT license.
