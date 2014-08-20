Description
-----------
Use the Webform Represent module to easily create email campaigns to elected
officials at all levels of government in Canada.

Represent [1] is the largest database of elected officials in Canada, and offers
the most comprehensive postal code lookup service for elected officials at the
federal, provincial and municipal levels of government.

This module integrates the Represent API with the Webform module by creating a
Represent Webform component, which includes the following features:

* Looks up a submitter's representatives from one or more levels of government.
* Performs a live AJAX lookup as soon as a postal code is entered.
* Offers the submitter a list of representatives if the lookup fails.

You can take advantage of the Webform module's core features to:

* Send a custom message to the submitter's representatives.
* Export the representative's name, district and email as Excel or CSV.
* Review and analyze the submitter's representatives online.

1. http://represent.opennorth.ca/

Requirements
------------

* http://drupal.org/project/webform
* http://drupal.org/project/represent

Installation
------------
1. Copy the entire webform_represent directory to the Drupal sites/all/modules
   directory.

2. Login as an administrator. Enable the module in the "Administer" -> "Site
   Building" -> "Modules"

3. Add Represent components to webform nodes and configure message recipients at
   admin/content/webform.

Support
-------
Please use the issue queue for filing bugs with this module at
http://drupal.org/project/issues/webform_represent
