This set of projects demonstrates how to use a BCB4-created DLL in a
BC++5.02-created MDI Doc/View application to display a view similar to
the TDialogView class (of OWL NExt) but actually a wrapper around a
VCL TForm using functions exported from the DLL.

Class             File         Library  Description
===============   ===========  =======  =======================
TVCLFormWrapper   VCLForm.cpp  OWL      A "wrapper" around any TForm decendent
                                        object in the DLL. Serves as the base
                                        class for all VCL/DLL-based dialogs in
                                        the application.  You must pass to the
                                        c'tor a function from the DLL to
                                        construct the body of the class there.
                                        See TVCLDialogView in VCLView.cpp for
                                        an example.

TVCLDialogView    VCLView.cpp  OWL      A "wrapper" around any
                                        TDialogViewBaseForm decendent object in
                                        the DLL. Derived from TVCLFormWrapper.
                                        Serves as the base class for all
                                        DLL-based views in the application.
                                        You must pass to the c'tor a function
                                        from the DLL to construct the body of
                                        the class there. See TExampleOwlView in
                                        ExOwlView.h for an example.

TVCLDialogView    DlgBase.cpp  VCL      The base class for all view classes in
                                        the DLL. You must derive your own classes
                                        and customize by adding whatever
                                        TControls you like. If you need to
                                        override FormCreate in a decendent
                                        class, be sure to call the parent
                                        FormCreate *after* performing necessary
                                        setup.  This will ensure that the
                                        DlgHasChanged flag is cleared properly.
                                        See TExampleView1 in ExView1.h for
                                        an example.

TVCLFileDocument  VCLDoc.cpp   OWL      A "wrapper" around any TBasicDocument
                                        decendent object in the DLL. Serves
                                        as the base class for all DLL-based
                                        documents in the application.  You must
                                        pass to the c'tor a function from the
                                        DLL to construct the body of the class
                                        there. See TExampleOwlDocument in
                                        ExOwlDoc.h for an example.

TBasicDocument    DocBase.cpp  VCL      The base class for all document classes
                                        in the DLL. You must derive your own
                                        classes and customize by overriding the
                                        get, put, SetDialogData, and
                                        GetDialogData virtual functions. See
                                        TExampleDocument1 in ExDoc1.h for an
                                        example.


These classes may be used without restrictions.
If you make improvements to these classes, I would greatly appreciate it
if you would send them to me.

Greg Bullock
Mission Research Corporation
bullock@mrcmry.com

