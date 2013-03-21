/* Handle 'I have viewed' button */
jQuery(document).ready(function(){
	jQuery('.xapi-viewed button').click(function() {
		jQuery(this).hide();
		var url = '/xapi/view' +
				'?entity=' + jQuery(this).attr('entity') +
				'&eid=' + jQuery(this).attr('eid') +
				'&verb=' + jQuery(this).attr('verb');
		jQuery.ajax(url);
	});
});
