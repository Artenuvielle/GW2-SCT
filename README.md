# Guild Wars 2 - Scrolling Combat Text (GW2SCT)

This is an addon for arcdps by deltaconnected (https://www.deltaconnected.com/arcdps/) and adds a highly customizable floating and scrolling combat text beside the users character. Incoming damage is displayed on the left side, outgoing damage is displayed on the right side.

### Prerequisites

GW2SCT requires ArcDPS to work. You can install arcdps by following the instructions in this site:
https://www.deltaconnected.com/arcdps/

### Installing

Go to the the [release page](https://github.com/Artenuvielle/GW2-SCT/releases) and download the newest release. Copy the file "d3d9_arcdps_sct.dll" into the "bin64/"" subfolder of your game directory.

If you want to add your own fonts copy them in the "addons/sct/fonts/" subfolder of your game directory. You can configure the output in the arcdps options panel (opened by default with Alt+Shift+T).

### Translations

Translations can be found on this repository in the according language folder. If you have a translations for a new language or improvements for one, get in touch with me.

Translations available and authors:
* english (complete, [Artenuvielle](https://github.com/Artenuvielle))
* chinese (mostly complete, [jiangyi0923](https://github.com/jiangyi0923))

## Contributing

Feel free to contact me on [Reddit](https://www.reddit.com/user/Artenuvielle/) if you have suggestions or encounter any bugs not listed below.

## Known Bugs

* The addon seems to not be loaded correctly or at all on some machines. If you have any information why this may be happening or how to resolve this, please reach out to me on [Reddit](https://www.reddit.com/user/Artenuvielle/)
* The default font loaded by this addon is also used as default font for arcdps. This means that the custom font size and type set up for arcdps will be ignored.

## Version History

* 1.2-RC1
	* Added translation support
	* Added support for chinese fonts
	* Added option for filtering out skills of a given id
	* Optimized message interpreting being run once per message before render and then on the fly only when options change (boosts FPS).
	* Fixed the mixup of damage and healing due to arcdps update
	* Fixed skill icons not being displayed for pets
* 1.1-RC3
	* Added support for ArcDPS API revision 1
	* Added list of skill ids to filter from all messages
* 1.1-RC2
	* Added skill icons as template parameter
	* Fixed %p for pet/clone messages
	* Added option to select master font
	* Added option to hide messages to yourself in outgoing window
* 1.1-RC1
	* Added options window (removed options dropdown in arcdps options window)
	* Added editable message templates
* 1.0
	* Fixed that enemy damage was shown as pet/clone damage
* 1.0-RC1
	*  initial release

## Authors

* **René Martin** - *Initial work* - [Artenuvielle](https://github.com/Artenuvielle)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

The source code includes a copy of the [imgui library](https://github.com/ocornut/imgui) which is also licensed under the MIT license.
