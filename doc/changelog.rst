
Change-log
===============================================================================

2.5.0-beta (27.10.2018)
-------------------------
- Added: Dialog DataRefs/Commands selector. I.e. buttons ``drf:`` and ``cmd:`` are implemented now.
- Added: Settings window. Use menu ``X-Plane->Settings`` for accessing it.
- Added: Ability to use tool-tip for ``ATTR_manip_noop``.
- Added: Ability to use text variables in the param light's parameters string.
- Updated: Link to the help. It is on-line now.
- Updated: Embedded named and param lights' names lists. Full list you can find here ``X-Plane-root/Resources/bitmaps/world/lites/lights.txt``
- Fixed: Sorting algorithm for LODs. `#9 <https://github.com/steptosky/3DsMax-XplnObj/issues/9>`_
- Fixed: Fixed bug for attribute printing:  ``AttrShiny`` ``AttrBlend``. Default/disabled values were not printed when those attributes were enabled and then disabled.

2.4.0-beta (12.09.2018)
-------------------------
- Added: Supporting for 3ds max 2018 and 2019.
- Improved: Some GUI elements/windows/dialogs.
- Fixed: LODs' values exporting. It isn't scaled anymore if you use system units other than 'meters'.

.. note::
    New Plug-in's dock window and menu aren't good adjusted to be used with the new 3Ds Max's workspaces conception.


2.3.1 (31.08.2018)
-------------------------
- Updated: Some manipulators UI text to be shorter.
- Updated: Limits for values of hide/show animation. 10 000 -> 1 000 000.
- Fixed: ``LIGHT_SPILL_CUSTOM`` direction export.
- Fixed: Signature printing.
- Fixed: Printing incorrect attribute name for the ``ATTR_manip_command_switch_up_down`` and ``ATTR_manip_command_switch_left_right``.


2.3.0 (05.10.2017)
-------------------------
- Added: Error if any LOD except the first one contains hard polygons.
- Added: Auto-flip normals for objects with mirrored transformation.
- Fixed: GUI for the light param.
- Fixed: Creating two sided mesh.
- Fixed: Printing custom name of the ``LIGHT_PARAM``.


2.2.1 (26.08.2017)
-------------------------
- Fixed: Printing error if texture name contains illegal symbols.
- Fixed: Opening update message for non x-plane scenes.
- Fixed: Crashing on some objects, for example 'Biped Object'.
- Fixed: Vertical scroll bar in the export console.


2.2.0 (10.07.2017)
-------------------------
- Modified: By default all the manipulators are disabled now! Previous behavior was: the 'panel' manipulator was implicitly set when you added any cockpit attribute like ``ATTR_cockpit``.

- Added: Panel-Click manipulator. This manipulator should be used for the cockpit geometry (`ATTR_cockpit`) when you want to enable the panel manipulators.
- Added: Checking if opening scene was created with a newer plug-in version. If so then the scene will not be loaded and a warning will be printed.
- Added: ``Smoke`` object. It can be found near ``X-Obj`` and ``X-Lod``.
- Added: 'X-Plane' menu on the 3Ds Max's top menu bar.
- Added: Documentation in the separated repository. Use the 'X-Plane' menu to get it.

- Fixed: Log file creation for some 3d max versions.
- Fixed: Manipulators' state machine. There were some problems in the state machine one of them could lead to undefined behavior, it was fixed.


2.1.0 (16.05.2017)
-------------------------
- Added: '(U) Update' buttons for the animation rollups, allows you to re-synchronize animation keys and values.
- Added: Tool-tips for the some controls of the export window.
- Added: Ability to enable or disable 'X-Obj' export. It helps when you work with more than one 'X-Obj' in the scene. You can export only selected ones.
- Added: Check-box for enabling auto-export. If it is enabled then the export will be auto-started without pressing the 'Export' button.

- Fixed: Link to donate. The button 'donate' leads to the correct web page now.
- Fixed: Loop animation attribute printing.


2.0.2-beta (23.04.2017)
-------------------------
- Added: Changing the text of the update button if a new version is available.
- Fixed: Potential unstable code.


2.0.1-beta (22.04.2017)
-------------------------
- Added: Auto-check for update. If a new version is available after exporting you will see the information in the console.

- Fixed: Incorrect error printing for the animation keys.
- Fixed: Cloning X-Plane object data during the objects clone in the 3Ds Max 2017.


2.0.0-beta (19.04.2017)
-------------------------
- Released: beta version.
