# Guild Wars 2 - Scrolling Combat Text (GW2SCT)

This is an addon for arcdps by deltaconnected (https://www.deltaconnected.com/arcdps/) and adds a highly customizable floating and scrolling combat text beside the users character. Incoming damage is displayed on the left side, outgoing damage is displayed on the right side.

### Prerequisites

GW2SCT requires ArcDPS to work. You can install arcdps by following the instructions in this site:
https://www.deltaconnected.com/arcdps/

### Installing

Go to the the [release page](https://github.com/Artenuvielle/GW2-SCT/releases) and download the newest release. Copy the file "d3d9_arcdps_sct.dll" into the "bin64/"" subfolder of your game directory.

If you want to add your own fonts copy them in the "addons/sct/fonts/" subfolder of your game directory. You can configure the output in the arcdps options panel (opened by default with Alt+Shift+T).

If for some reason the addon is not loading for you, check that you have the newest version of the Microsoft Visual C++ Redistributables installed. Get it [here](https://support.microsoft.com/de-de/help/2977003/the-latest-supported-visual-c-downloads).

### Translations

Translations can be found on this repository in the according language folder. If you have a translations for a new language or improvements for one, get in touch with me.

Translations available and authors:
* english (complete for 2.0-RC3, [Artenuvielle](https://github.com/Artenuvielle))
* chinese (only complete for 1.1-RC3, [jiangyi0923](https://github.com/jiangyi0923))
* french (only complete for 1.2-RC3, [livarkhal](https://github.com/livarkhal))

If you want to check which translation strings were changed in which version have a look [here](https://github.com/Artenuvielle/GW2-SCT/blob/master/languages/LANGUAGE_CHANGELOG.md).

## Contributing

Feel free to contact me on [Reddit](https://www.reddit.com/user/Artenuvielle/) if you have suggestions or encounter any bugs not listed below.

## Known Bugs

* When having problems accessing the GW2 render API no skill icons can be downloaded or it takes a very long time. Download and extract the images in icons.zip file from [here](https://github.com/Artenuvielle/GW2-SCT/issues/11#issuecomment-606794158) and into the "addons/sct/icons" subfolder of your game directory.

## Version History

* 2.0-RC3 (WIP)
	* profile system (have multiple profiles of setting to quickly switch between and also have character bound profiles)
	* transparency option for skill icons
	* recording and playback of messages for easier option adjustment
	* still planned for 2.0: replace other persistence file formats (language, skill remaps and skill icons)
* 2.0-RC2
	* published source code
	* output messages to multiple scroll areas
	* switched options persistence format
	* added %r parameter into template system for profession names
* 2.0-RC1
	* added own font rendering system (problems with other addons font scaling should be fixed)
	* changed memory pool for textures (this caused the game to freeze when resizing game window previously)
* 1.3-RC4
	* updated multiple libraries used
	* fixed skill filter options tab
	* fixed game crashing if for any reason the GW2 rendering API was not reachable
* 1.3-RC3
	* fixed color pickers not working in message options tab
	* fixed active options tab not being highlighted in menu bar
* 1.3-RC2
	* updated imgui library to version 1.80
	* fixed compatibility with arcdps exports (feb.23.2021 changes)
* 1.3-RC1
	* added options for changing default font size and default crit font size
	* improved skill filtering (all previously filtered skill ids won't work any more, please readd them manually)
	* added a couple informational tooltips
	* added posibility to read in a remap.ini for remapping skill ids to different ones for retrieving the correct icons (in the ini under [Active] section add key values like '46469 = 9168'; this e.g. fixes [SoJ](https://github.com/Artenuvielle/GW2-SCT/issues/9#issuecomment-604472445))
	* fixed a bug in language file loading
	* fixed a bug when deleting first scroll area in list
	* fixed a bug when deleting first filtered skill id in list
	* improved debug logging for skill icons download
* 1.2-RC3
	* Changed skill icon loading code to be compatible with d912pxy
* 1.2-RC2
	* Improved loading of language files
	* Increased maximal scrolling speed for supporting higher display resolutions.
	* Small tweak to skill icon loading to maybe fix some issues.
* 1.2-RC1
	* Added scroll areas
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
