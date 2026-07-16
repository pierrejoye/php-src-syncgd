<?php
if (!GD_BUNDLED && version_compare(GD_VERSION, '2.4.0', '<')) {
    die('skip requires bundled libgd or external libgd >= 2.4.0');
}
