# Language file Changelog

Here you can find all language strings added for any given version if you want to improve a language file.

## Version History

* 2.0-RC3
	* switched to json file format
	* all categories containing message parameter descriptions or default templates were renamed and any of their keys can now be omitted, if a key is omitted this way its value is first searched in the definitions for the according message type, then message catagory and then general options (e.g. `[Message_Pet_Out_Heal] -> Parameter_Description_s` will take the first value of `[Message_Type_Heal] -> Parameter_Description_s`, `[Message_Category_Pet_Out] -> Parameter_Description_s` or `[Message_General] -> Parameter_Description_s`)
	* Added:
		* `[Option_UI] -> Menu_Bar_Profiles`
		* `[Profile_Option_UI] -> Delete_Confirmation_Title`
		* `[Profile_Option_UI] -> Delete_Confirmation_Content`
		* `[Profile_Option_UI] -> Delete_Confirmation_Confirmation`
		* `[Profile_Option_UI] -> Delete_Confirmation_Cancel`
		* `[Profile_Option_UI] -> Profile_Description`
		* `[Profile_Option_UI] -> Master_Profile`
		* `[Profile_Option_UI] -> Character_Specific_Profile_Heading`
		* `[Profile_Option_UI] -> Character_Specific_Profile_Enabled`
		* `[Profile_Option_UI] -> Current_Profile_Heading`
		* `[Profile_Option_UI] -> Current_Profile_Name`
		* `[Profile_Option_UI] -> Create_Profile_Copy`
		* `[Profile_Option_UI] -> Delete_Profile`
		* `[Profile_Option_UI] -> Profile_Copy_Suffix`
		* `[Skill_Filter_Option_UI] -> Filter_By`
		* `[Skill_Icons_Option_UI] -> Skill_Icons_Display_Type`
		* `[Skill_Icons_Option_UI] -> Skill_Icons_Display_Type_Normal`
		* `[Skill_Icons_Option_UI] -> Skill_Icons_Display_Type_Black_Culled`
		* `[Skill_Icons_Option_UI] -> Skill_Icons_Display_Type_Black_Border_Culled`
		* `[Skill_Icons_Option_UI] -> Skill_Icons_Display_Type_Black_Border_Touching_Culled`
	* Moved:
		* `[Option_UI] -> Skill_Icons_Warning` to `[Skill_Icons_Option_UI] -> Skill_Icons_Warning`
		* `[Option_UI] -> Skill_Icons_Enable` to `[Skill_Icons_Option_UI] -> Skill_Icons_Enable`
		* `[Option_UI] -> Skill_Icons_Preload_Description` to `[Skill_Icons_Option_UI] -> Skill_Icons_Preload_Description`
		* `[Option_UI] -> Skill_Icons_Preload` to `[Skill_Icons_Option_UI] -> Skill_Icons_Preload`
	* Removed:
		* `[Option_UI] -> Menu_Bar_Messages`
* 2.0-RC2
	* Added:
		* `[General] -> Font_Size_Type`
		* `[General] -> Default_Font_Size`
		* `[General] -> Default_Crit_Font_Size`
		* `[General] -> Custom_Font_Size`
		* `[General] -> Skill_Filter_Type_ID`
		* `[General] -> Skill_Filter_Type_Name`
		* `[Option_UI] -> Menu_Bar_Filtered_Skills`
		* `[Option_UI] -> General_Scrolling_Speed_Toolip`
		* `[Option_UI] -> General_Max_Messages_Toolip`
		* `[Option_UI] -> General_Combine_Messages_Toolip`
		* `[Option_UI] -> General_Self_Only_As_Incoming_Toolip`
		* `[Option_UI] -> General_Out_Only_For_Target_Toolip`
		* `[Option_UI] -> Scroll_Areas_Name`
		* `[Option_UI] -> Receiver_Name`
		* `[Option_UI] -> Messages_Type`
		* `[Option_UI] -> Messages_Type_Physical`
		* `[Option_UI] -> Messages_Type_Crit`
		* `[Option_UI] -> Messages_Type_Bleeding`
		* `[Option_UI] -> Messages_Type_Burning`
		* `[Option_UI] -> Messages_Type_Poison`
		* `[Option_UI] -> Messages_Type_Confusion`
		* `[Option_UI] -> Messages_Type_Retaliation`
		* `[Option_UI] -> Messages_Type_Torment`
		* `[Option_UI] -> Messages_Type_Dot`
		* `[Option_UI] -> Messages_Type_Heal`
		* `[Option_UI] -> Messages_Type_Hot`
		* `[Option_UI] -> Messages_Type_Shield_Receive`
		* `[Option_UI] -> Messages_Type_Shield_Remove`
		* `[Option_UI] -> Messages_Type_Block`
		* `[Option_UI] -> Messages_Type_Evade`
		* `[Option_UI] -> Messages_Type_Invulnerable`
		* `[Option_UI] -> Messages_Type_Miss`
		* `[Scroll_Area_Option_UI] -> All_Receivers`
		* `[Scroll_Area_Option_UI] -> New_Receiver`
		* for incoming and incoming pet message category and type sections: `[...] -> Parameter_Description_r`
	* Moved:
		* `[Option_UI] -> Scroll_Areas_New` to `[Scroll_Area_Option_UI] -> Scroll_Areas_New`
	* Updated:
		* `[General] -> Text_Align_Center`
		* `[Scroll_Area_Option_UI] -> Delete_Confirmation_Content`
	* Removed:
		* `[Option_UI] -> Menu_Bar_Filtered_Skill_IDs`
		* `[Option_UI] -> Scroll_Areas_None_Exist`
		* for all message category and type sections: `[...] -> title`
* 1.3-RC1
	* Added:
		* `[General] -> General_Scrolling_Speed_Toolip`
		* `[General] -> General_Max_Messages_Toolip`
		* `[General] -> General_Combine_Messages_Toolip`
		* `[General] -> General_Self_Only_As_Incoming_Toolip`
		* `[General] -> General_Out_Only_For_Target_Toolip`
		* `[General] -> Font_Size_Type`
		* `[General] -> Default_Font_Size`
		* `[General] -> Default_Crit_Font_Size`
		* `[General] -> Custom_Font_Size`
		* `[Option_UI] -> Menu_Bar_Filtered_Skills`
	* Removed:
		* `[Option_UI] -> Menu_Bar_Filtered_Skill_IDs`
* 1.1-RC3
	* initially started language files
