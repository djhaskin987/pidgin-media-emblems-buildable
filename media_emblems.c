/*
    This file is part of Pidgin Media Emblems Plugin -- Buildable Edition.

    Pidgin Media Emblems Plugin -- Buildable Edition is free software: you can
    redistribute it and/or modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    Pidgin Media Emblems Plugin -- Buildable Edition is distributed in the hope
    that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
    warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Pidgin Media Emblems Plugin -- Buildable Edition.
    If not, see <http://www.gnu.org/licenses/>.
*/
#define PURPLE_PLUGINS

#include <glib.h>
#include <plugin.h>
#include <debug.h>
#include <account.h>
#include <accountopt.h>

#ifndef _
#	define _(a) a
#endif

static GHashTable *original_emblems = NULL;
const char *media_emblem(PurpleBuddy *buddy);

const char *
media_emblem(PurpleBuddy *buddy)
{
	const char*(* emblem_func)(PurpleBuddy *buddy);
	const char *prpl_id;
	const char *emblem;
	PurpleAccount *account;
	PurpleMediaCaps mediacaps;
	
	if (buddy != NULL)
		account = buddy->account;
	else
		return NULL;
	
	prpl_id = purple_account_get_protocol_id(account);
	emblem_func = g_hash_table_lookup(original_emblems, prpl_id);
	if (emblem_func)
	{
		emblem = emblem_func(buddy);
		if (emblem)
			return emblem;
	}
	
	mediacaps = purple_prpl_get_media_caps(account, buddy->name);
	
	if (mediacaps & (PURPLE_MEDIA_CAPS_AUDIO_VIDEO | PURPLE_MEDIA_CAPS_VIDEO | PURPLE_MEDIA_CAPS_VIDEO_SINGLE_DIRECTION))
		return "video";
	if (mediacaps & (PURPLE_MEDIA_CAPS_AUDIO | PURPLE_MEDIA_CAPS_AUDIO_SINGLE_DIRECTION))
		return "voice";
	
	return NULL;
}

static gboolean
plugin_load(PurplePlugin *plugin)
{
	GList *proto_list;
	PurplePlugin *proto;
	PurplePluginProtocolInfo *prpl_info;
	
	//Hook into every protocol's prpl_info->list_emblem function
	original_emblems = g_hash_table_new(g_str_hash, g_str_equal);
	
	proto_list = purple_plugins_get_protocols();
	
	for(; proto_list; proto_list = proto_list->next)
	{
		proto = proto_list->data;
		prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(proto);
	
		g_hash_table_insert(original_emblems, (gpointer)purple_plugin_get_id(proto), prpl_info->list_emblem);
		prpl_info->list_emblem = media_emblem;
	}
	
	return TRUE;
}

static gboolean
plugin_unload(PurplePlugin *plugin)
{
	//Unhook
	GList *proto_list;
	gpointer func;
	PurplePluginProtocolInfo *prpl_info;
	PurplePlugin *proto;
	
	proto_list = purple_plugins_get_protocols();
	for(; proto_list; proto_list = proto_list->next)
	{
		proto = (PurplePlugin *) proto_list->data;
		prpl_info = PURPLE_PLUGIN_PROTOCOL_INFO(proto);
		func = g_hash_table_lookup(original_emblems, purple_plugin_get_id(proto));
		if (func)
		{
			prpl_info->list_emblem = func;
			g_hash_table_remove(original_emblems, purple_plugin_get_id(proto));
		} else if (prpl_info->list_emblem == media_emblem)
		{
			prpl_info->list_emblem = NULL;
		}
	}
	g_hash_table_destroy(original_emblems);
	original_emblems = NULL;
	
	return TRUE;
}

static void
plugin_init(PurplePlugin *plugin)
{
}

static PurplePluginInfo info = 
{
	PURPLE_PLUGIN_MAGIC,
	2,
	2,
	PURPLE_PLUGIN_STANDARD,
	NULL,
	0,
	NULL,
	PURPLE_PRIORITY_LOWEST,

	"eionrobb-media-emblem",
	"Media Emblems",
	"0.1",
	"Show video/audio emblems for buddies",
	"Displays emblems in the buddy list if a buddy supports an audio or video call",
	"Eion Robb <eionrobb@gmail.com>",
	"http://pidgin-media-emblems.googlecode.com/", //URL
	
	plugin_load,
	plugin_unload,
	NULL,
	
	NULL,
	NULL,
	NULL,
	NULL,
	
	NULL,
	NULL,
	NULL,
	NULL
};

PURPLE_INIT_PLUGIN(icon_override, plugin_init, info);
